
#include <Camera.h>
#include <dlfcn.h>

CameraHandler::CameraHandler()
{
	LogDebug("CameraHandler::CameraHandler");
	gst_init (0, NULL);

	guint major, minor, micro, nano;
	gst_version(&major, &minor, &micro, &nano);
	LogInfo("Gstreamer Version: %u.%u.%u.%u", major, minor, micro, nano);
	
	WServer = NULL;
	RServer = NULL;
	m_dll = NULL;

}

CameraHandler::~CameraHandler()
{
	LogDebug("CameraHandler::~CameraHandler");

	//Cleanup Video Inputs's
	while(m_VideoInputs.size() > 0)
	{
		std::map<unsigned int, struct VideoInputConfig *>::iterator it = m_VideoInputs.begin();
		int input = it->first;
		VideoInputConfig *config = it->second;
		if (config->GetEnabled())
			m_Platform->VideoInputDisable(input);
		m_VideoInputs.erase(it);
		delete config;
	}

	WServer->Stop();
	delete WServer;

	User::Destroy();

	delete Cfg;
	delete m_Platform;
	delete RServer;
	
	if (m_dll)
		dlclose(m_dll);
	
	gst_deinit();
}

static bool CheckDLL(const std::string *path)
{
	struct stat info;
	LogDebug("Checking DLL '%s'", path->c_str());
	if (stat(path->c_str(), &info) == 0)
		return true;
	return false;
}

static bool FindDLL(const std::string &Platform, std::string *path)
{
	*path = Platform;
	if (CheckDLL(path))
		return true;
	
	*path = "lib" + Platform + ".so";
	if (CheckDLL(path))
		return true;
		
	*path = "camera-1.0/lib" + Platform + ".so";
	if (CheckDLL(path))
		return true;

	*path = "/usr/lib/camera-1.0/lib" + Platform + ".so";
	if (CheckDLL(path))
		return true;

	*path = "/usr/local/lib/camera-1.0/lib" + Platform + ".so";
	if (CheckDLL(path))
		return true;

	/* FIXME: This needs removed and replace with proper path + env searchs */		
	*path = "../platforms/example/.libs/lib" + Platform + ".so";
	if (CheckDLL(path))
		return true;

	return false;	
}

//Init Does not need locking as it should be the only code active in the system during startup
void CameraHandler::Init(const std::string Platform, const std::string CfgFile)
{
	LogInfo("Version: %s", Version::ToString().c_str());
	LogDebug("CameraHandler::Init(\"%s\", \"%s\")", Platform.c_str(), CfgFile.c_str());
	m_CfgFile = CfgFile;

	//Start Various Services
	WServer = new WebServer();
	RServer = new RTSPServer();

	//Search for platform dll
	std::string dll = "";
	if (FindDLL(Platform, &dll) == false)
	{
		if (FindDLL(String::ToLower(Platform), &dll) == false)
		{
			LogCritical("Cannot find dll for platform '%s'", Platform.c_str());
			exit(EXIT_FAILURE);
		}
	}

	m_dll = dlopen(dll.c_str(), RTLD_NOW);
	if (m_dll == NULL)
	{
		LogCritical("Fail to load dll: %s", dll.c_str());
		LogCritical("Error: %s", dlerror());
		exit(EXIT_FAILURE);
	}
	
	PlatformBase *(*create)() = (PlatformBase *(*)()) dlsym(m_dll, "Create");
	if (create == NULL)
	{
		LogCritical("DLL '%s' does not contain a symbol for 'Create'", dll.c_str());
		exit(EXIT_FAILURE);
	}
	

	m_Platform = create();
	if (m_Platform == NULL)
	{
		LogCritical("Fail to load platform: %s", Platform.c_str());
		exit(EXIT_FAILURE);
	}

	if (m_Platform->Init() == false)
	{
		LogCritical("Failed To Init Platform exiting....");
		exit(EXIT_FAILURE);
	}

	if (User::Init() == false)
	{
		LogCritical("Failed to Init user database....");
		exit(EXIT_FAILURE);
	}

	//Dump out some debug info about how many streams we support
	unsigned int nInputs = m_Platform->VideoInputCount();
	LogInfo("Supported Video Inputs: %d", nInputs);
	for(unsigned int i = 0; i < nInputs; i++)
	{
		VideoInputSupported info;
		info.Clear();
		if (m_Platform->VideoInputSupportedInfo(i, &info) == false)
		{
			LogError("Failure to VideoInputSupportedInfo(%d)", i);
			exit(EXIT_FAILURE);
		}
		LogInfo("VideoInput %u Supports", i);
		info.LogDump();
	}

	//Load Default Config's. The Config Load can override these later
	//We also wait until after the config load until we enable the config + enable the streams on the platform.
	for(unsigned int i = 0; i < nInputs; i++)
	{
		//Load Up Default Configs
		VideoInputConfig *config = new VideoInputConfig();
		m_Platform->VideoInputDefaultConfig(i, config);
		LogInfo("VideoInput %u default Config: '%s'", i, config->ToString().c_str());
		m_VideoInputs[i] = config;
	}


	//Load The actual configuration
	Cfg = new Config(this, CfgFile);

	if (Cfg->Load() == false)
	{
		LogCritical("Failed To Load Config File: \"%s\" exiting ....", m_CfgFile.c_str());
		exit(EXIT_FAILURE);
	}


	//Start The Video Inputs
	std::map<unsigned int, struct VideoInputConfig *>::iterator it = m_VideoInputs.begin();
	while(it != m_VideoInputs.end())
	{
		if (m_Platform->VideoInputConfigure(it->first, it->second) == false)
		{
			LogError("CameraHandler::Init Failed to Configure Input %u Config: %s", it->first, it->second->ToString().c_str());
			abort();
		}
		if (it->second->GetEnabled())
		{
			if (VideoInputEnable(it->first) == false)
			{
				LogError("CameraHandler::Init Failed to Enable VideoInput %u", it->first);
				abort();
			}
		}
		it++;
	}
	
	//Load WebService Last
	WServer->Start();
}

bool CameraHandler::ConfigLoad(Json::Value &json)
{
	LogDebug("CameraHandler::ConfigLoad");
	ScopedLock Lock = ScopedLock(&m_ConfigMutex);

	if (json.isMember("debug"))
		if (Debug::ConfigLoad(json["debug"]) == false)
			return false;

	if (json.isMember("users"))
		if (User::ConfigLoad(json["users"]) == false)
			return false;

	if (json.isMember("group"))
		if (Group::ConfigLoad(json["groups"]) == false)
			return false;

	if (json.isMember("platform"))
		if (m_Platform->ConfigSave(json["platform"]) == false)
			return false;

	if (json.isMember("rtspserver"))
		if (RServer->ConfigLoad(json["rtspserver"]) == false)
			return false;
			
	if (json.isMember("webserver"))
		if (WServer->ConfigLoad(json["webserver"]) == false)
			return false;

	ScopedLock VideoLock(&m_VideoInputMutex);
	std::map<unsigned int, struct VideoInputConfig *>::iterator it = m_VideoInputs.begin();
	while(it != m_VideoInputs.end())
	{
		std::stringstream ss;
		ss << "VideoInputConfig_" << it->first;
		if (json.isMember(ss.str()))
		{
			if (it->second->ConfigLoad(json[ss.str()]) == false)
			{
				LogWarning("CameraHandler::ConfigLoad - Failed to load configuration for video input '%s'", ss.str().c_str());
				return false;
			}
			LogInfo("VideoInput %u loaded config '%s'", it->first, it->second->ToString().c_str());
		}
		else
		{
			LogWarning("CameraHandler::ConfigLoad - No configuration for video input '%s'", ss.str().c_str());
		}
		it++;
	}

	return true;
}

bool CameraHandler::ConfigSave(Json::Value &json)
{
	LogDebug("CameraHandler::ConfigSave");
	ScopedLock Lock = ScopedLock(&m_ConfigMutex);

	json["Version"] = Version::ToString();

	if (Debug::ConfigSave(json["debug"]) == false)
		return false;

	if (m_Platform->ConfigSave(json["platform"]) == false)
		return false;

	if (RServer->ConfigSave(json["rtspserver"]) == false)
		return false;

	ScopedLock VideoLock = ScopedLock(&m_VideoInputMutex);
	std::map<unsigned int, struct VideoInputConfig *>::iterator it = m_VideoInputs.begin();
	while(it != m_VideoInputs.end())
	{
		std::stringstream ss;
		ss << "VideoInputConfig_" << it->first;
		if (it->second->ConfigSave(json[ss.str()]) == false)
		{
			LogError("CameraHandler::ConfigSave Failed to log configuration for video input '%s'", ss.str().c_str());
			return false;
		}
		it++;
	}

	if (User::ConfigSave(json["users"]) == false)
		return false;

	if (Group::ConfigSave(json["groups"]) == false)
		return false;

	if (WServer->ConfigSave(json["webserver"]) == false)
		return false;

	return true;
}

void CameraHandler::VideoInputCount(int *count)
{
	LogDebug("CameraHandler::VideoInputCount");
	*count = m_Platform->VideoInputCount();
}

bool CameraHandler::VideoInputSetEnabled(unsigned int input, bool enabled)
{
	LogDebug("CameraHandler::VideoInputSetEnabled(%u, %s)", input, enabled ? "True" : "False");
	ScopedLock VideoLock = ScopedLock(&m_VideoInputMutex);
	std::map<unsigned int, struct VideoInputConfig *>::iterator it = m_VideoInputs.find(input);
	if (it == m_VideoInputs.end())
	{
		LogDebug("CameraHandler::VideoInputSetEnabled(%u, %s) - No such input", input, enabled ? "True" : "False");
		return false;
	}


	bool orig = m_VideoInputs[input]->GetEnabled();

	if (orig == false && enabled == true)
	{
		m_VideoInputs[input]->SetEnabled(true);
		return VideoInputEnable(input);
	}

	if (orig == true && enabled == false)
	{
		m_VideoInputs[input]->SetEnabled(false);
		return VideoInputDisable(input);
	}

	LogError("CameraHandler::VideoInputSetEnabled() - Did Nothing - Probably a bug in the client...");
	return true;
}

bool CameraHandler::VideoInputGetEnabled(unsigned int input, bool &enabled)
{
	LogDebug("CameraHandler::VideoStreamGetEnabled(%u)", input);
	ScopedLock VideoLock = ScopedLock(&m_VideoInputMutex);
	std::map<unsigned int, struct VideoInputConfig *>::iterator it = m_VideoInputs.find(input);
	if (it == m_VideoInputs.end())
	{
		LogDebug("CameraHandler::VideoInputGetEnabled(%u) - No such input", input);
		return false;
	}

	enabled = m_VideoInputs[input]->GetEnabled();
	return true;
}

bool CameraHandler::VideoInputEnable(unsigned int input)
{
	LogDebug("CameraHandler::VideoInputEnable(%u)", input);
	ScopedLock VideoLock = ScopedLock(&m_VideoInputMutex);
	std::map<unsigned int, struct VideoInputConfig *>::iterator it = m_VideoInputs.find(input);
	if (it == m_VideoInputs.end())
	{
		LogDebug("CameraHandler::VideoInputEnable(%u) - No such input", input);
		return false;
	}
	

	std::stringstream url;
	url << "/video/" << input;

	if (m_Platform->VideoInputEnable(input) == false)
	{
		LogError("Platform Failed to enabled video input %u", input);
		return false;
	}
	if (m_VideoInputs[input]->GetCodec() == "H264")
	{
		std::stringstream pipe;
		pipe << "( internalsrc streamname=video" << input << " ! rtph264pay name=pay0 pt=96 )";

		RServer->PipelineAdd(url.str().c_str(), pipe.str().c_str());
	}
	else
	{
		LogCritical("Unknown Codec: %s", m_VideoInputs[input]->GetCodec().c_str());
		abort();
	}
	return true;
}

bool CameraHandler::VideoInputDisable(unsigned int input)
{
	LogDebug("CameraHandler::VideoInputDisable(%u)", input);
	std::map<unsigned int, struct VideoInputConfig *>::iterator it = m_VideoInputs.find(input);
	if (it == m_VideoInputs.end())
	{
		LogDebug("CameraHandler::VideoInputDisable(%u) - No such input", input);
		return false;
	}

	std::stringstream url;
	url << "/video/" << input;

	RServer->PipelineRemove(url.str().c_str());
	if (m_Platform->VideoInputDisable(input) == false)
	{
		LogError("Platform Failed to disable video input %u", input);
		return false;
	}
	return true;
}

void CameraHandler::Wait()
{
	LogDebug("CameraHandler::Wait");
	LogInfo("System Running");
	m_QuitBarrier.Wait();
}

void CameraHandler::Quit()
{
	LogDebug("CameraHandler::Quit");
	Cfg->Dirty();
	m_QuitBarrier.WakeUp();
}


