
#include <Camera.h>

CameraHandler::CameraHandler()
{
	LogDebug("CameraHandler::CameraHandler");
	gst_init (0, NULL);
}

CameraHandler::~CameraHandler()
{
	LogDebug("CameraHandler::~CameraHandler");

	//Cleanup Video Stream's
	while(m_VideoStreams.size() > 0)
	{
		std::map<unsigned int, struct VideoStreamConfig *>::iterator it = m_VideoStreams.begin();
		int stream = it->first;
		VideoStreamConfig *config = it->second;
		if (config->GetEnabled())
			m_Platform->VideoStreamDisable(stream);
		m_VideoStreams.erase(it);
		delete config;
	}

	delete Cfg;
	delete m_Platform;
	delete RServer;
	gst_deinit();
}

void CameraHandler::Init(const std::string Platform, const std::string CfgFile)
{
	LogInfo("Version: %s", Version::ToString().c_str());
	LogDebug("CameraHandler::Init(\"%s\", \"%s\")", Platform.c_str(), CfgFile.c_str());
	m_CfgFile = CfgFile;

	//Start RTSP Service
	RServer = new RTSPServer();

	m_Platform = Platform::Create(Platform);
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

	//Dump out some debug info about how many streams we support
	unsigned int nStreams = m_Platform->VideoStreamCount();
	LogInfo("Supported Video Streams: %d", nStreams);
	for(unsigned int i = 0; i < nStreams; i++)
	{
		VideoStreamSupported info;
		info.Clear();
		if (m_Platform->VideoStreamSupportedInfo(i, &info) == false)
		{
			LogError("Failure to VideoStreamSupportedInfo(%d)", i);
			exit(EXIT_FAILURE);
		}
		LogInfo("Stream %u Supports", i);
		info.LogDump();
	}

	//Load Default Config's. The Config Load can override these later
	//We also wait until after the config load until we enable the config + enable the streams on the platform.
	for(unsigned int i = 0; i < nStreams; i++)
	{
		//Load Up Default Configs
		VideoStreamConfig *config = new VideoStreamConfig();
		m_Platform->VideoStreamDefaultConfig(i, config);
		LogInfo("VideoStream %u default Config: '%s'", i, config->ToString().c_str());
		m_VideoStreams[i] = config;
	}


	//Load The actual configuration
	Cfg = new Config(this, CfgFile);

	if (Cfg->Load() == false)
	{
		LogCritical("Failed To Load Config File: \"%s\" exiting ....", m_CfgFile.c_str());
		exit(EXIT_FAILURE);
	}


	//Start The Video Streams
	std::map<unsigned int, struct VideoStreamConfig *>::iterator it = m_VideoStreams.begin();
	while(it != m_VideoStreams.end())
	{
		if (m_Platform->VideoStreamConfigure(it->first, it->second) == false)
		{
			LogError("CameraHandler::Init Failed to Configure Stream %u Config: %s", it->first, it->second->ToString().c_str());
			abort();
		}
		if (it->second->GetEnabled())
		{
			if (VideoStreamEnable(it->first) == false)
			{
				LogError("CameraHandler::Init Failed to Enable Stream %u", it->first);
				abort();
			}
		}
		it++;
	}
}

bool CameraHandler::ConfigLoad(Json::Value &json)
{
	LogDebug("CameraHandler::ConfigLoad");

	if (json.isMember("platform"))
		if (m_Platform->ConfigSave(json["platform"]) == false)
			return false;

	if (json.isMember("rtspserver"))
		if (RServer->ConfigLoad(json["rtspserver"]) == false)
			return false;

	std::map<unsigned int, struct VideoStreamConfig *>::iterator it = m_VideoStreams.begin();
	while(it != m_VideoStreams.end())
	{
		std::stringstream ss;
		ss << "VideoStreamConfig_" << it->first;
		if (json.isMember(ss.str()))
		{
			if (it->second->ConfigLoad(json[ss.str()]) == false)
			{
				LogWarning("CameraHandler::ConfigLoad - Failed to load configuration for video stream '%s'", ss.str().c_str());
				return false;
			}
			LogInfo("VideoStream %u loaded config '%s'", it->first, it->second->ToString().c_str());
		}
		else
		{
			LogWarning("CameraHandler::ConfigLoad - No configuration for video stream '%s'", ss.str().c_str());
		}
		it++;
	}

	return true;
}

bool CameraHandler::ConfigSave(Json::Value &json)
{
	LogDebug("CameraHandler::ConfigSave");

	json["Version"] = Version::ToString();
	
	if (m_Platform->ConfigSave(json["platform"]) == false)
		return false;
	
	if (RServer->ConfigSave(json["rtspserver"]) == false)
		return false;

	std::map<unsigned int, struct VideoStreamConfig *>::iterator it = m_VideoStreams.begin();
	while(it != m_VideoStreams.end())
	{
		std::stringstream ss;
		ss << "VideoStreamConfig_" << it->first;
		if (it->second->ConfigSave(json[ss.str()]) == false)
		{
			LogError("CameraHandler::ConfigSave Failed to log configuration for video stream '%s'", ss.str().c_str());
			return false;
		}
		it++;
	}

	return true;
}

bool CameraHandler::VideoStreamSetEnabled(unsigned int stream, bool enabled)
{
	LogDebug("CameraHandler::VideoStreamSetEnabled(%u, %s)", stream, enabled ? "True" : "False");
	std::map<unsigned int, struct VideoStreamConfig *>::iterator it = m_VideoStreams.find(stream);
	if (it == m_VideoStreams.end())
	{
		LogDebug("CameraHandler::VideoStreamSetEnabled(%u, %s) - No such stream", stream, enabled ? "True" : "False");
		return false;
	}


	bool orig = m_VideoStreams[stream]->GetEnabled();

	if (orig == false && enabled == true)
	{
		m_VideoStreams[stream]->SetEnabled(true);
		return VideoStreamEnable(stream);
	}

	if (orig == true && enabled == false)
	{
		m_VideoStreams[stream]->SetEnabled(false);
		return VideoStreamDisable(stream);
	}

	LogError("CameraHandler::VideostreamSetEnabled() - Did Nothing - Probably a bug in the client...");
	return true;
}

bool CameraHandler::VideoStreamGetEnabled(unsigned int stream, bool &enabled)
{
	LogDebug("CameraHandler::VideoStreamGetEnabled(%u)",stream);
	std::map<unsigned int, struct VideoStreamConfig *>::iterator it = m_VideoStreams.find(stream);
	if (it == m_VideoStreams.end())
	{
		LogDebug("CameraHandler::VideoStreamGetEnabled(%u) - No such stream", stream);
		return false;
	}

	enabled = m_VideoStreams[stream]->GetEnabled();
	return true;
}

bool CameraHandler::VideoStreamEnable(unsigned int stream)
{
	LogDebug("CameraHandler::VideoStreamEnable(%u)", stream);
	std::map<unsigned int, struct VideoStreamConfig *>::iterator it = m_VideoStreams.find(stream);
	if (it == m_VideoStreams.end())
	{
		LogDebug("CameraHandler::VideoStreamEnable(%u) - No such stream", stream);
		return false;
	}
	

	std::stringstream url;
	url << "/video/" << stream;

	if (m_Platform->VideoStreamEnable(stream) == false)
	{
		LogError("Platform Failed to enabled video stream %u", stream);
		return false;
	}
	if (m_VideoStreams[stream]->GetCodec() == "H264")
	{
		std::stringstream pipe;
		pipe << "( internalsrc streamname=video" << stream << " ! rtph264pay name=pay0 pt=96 )";

		RServer->PipelineAdd(url.str().c_str(), pipe.str().c_str());
	}
	else
	{
		LogCritical("Unknown Codec: %s", m_VideoStreams[stream]->GetCodec().c_str());
		abort();
	}
	return true;
}

bool CameraHandler::VideoStreamDisable(unsigned int stream)
{
	LogDebug("CameraHandler::VideoStreamDisable(%u)", stream);
	std::map<unsigned int, struct VideoStreamConfig *>::iterator it = m_VideoStreams.find(stream);
	if (it == m_VideoStreams.end())
	{
		LogDebug("CameraHandler::VideoStreamDisable(%u) - No such stream", stream);
		return false;
	}

	std::stringstream url;
	url << "/video/" << stream;

	RServer->PipelineRemove(url.str().c_str());
	if (m_Platform->VideoStreamDisable(stream) == false)
	{
		LogError("Platform Failed to disable video stream %u", stream);
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


