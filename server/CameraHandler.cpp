
#include <Camera.h>

CameraHandler::CameraHandler()
{
	LogDebug("CameraHandler::RTSPHandler");
	gst_init (0, NULL);
}

CameraHandler::~CameraHandler()
{
	LogDebug("CameraHandler::~RTSPHandler");
	delete m_Config;
	delete m_Platform;
	delete m_RServer;
	gst_deinit();
}

void CameraHandler::Init(const std::string Platform, const std::string CfgFile)
{
	LogInfo("Version: %s", Version::ToString().c_str());
	LogDebug("CameraHandler::Init(\"%s\", \"%s\")", Platform.c_str(), CfgFile.c_str());
	m_CfgFile = CfgFile;

	//Start RTSP Service
	m_RServer = new RTSPServer();

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
		LogInfo("Stream %d Supports", i);
		info.LogDump();
	}

	m_Config = new Config(this, CfgFile);

	if (m_Config->Load() == false)
	{
		LogCritical("Failed To Load Config File: \"%s\" exiting ....", m_CfgFile.c_str());
		exit(EXIT_FAILURE);
	}
}

bool CameraHandler::ConfigLoad(Json::Value &json)
{
	LogDebug("CameraHandler::ConfigLoad");

	if (json.isMember("platform"))
		if (m_Platform->ConfigSave(json["platform"]) == false)
			return false;

	if (json.isMember("rtspserver"))
		if (m_RServer->ConfigLoad(json["rtspserver"]) == false)
			return false;

	//FIXME: Remove .... Temporary the Platform Should do this
	m_RServer->PipelineAdd("/test", "( videotestsrc horizontal-speed=5 is-live=true ! capsfilter caps=capsfilter caps=\"video/x-raw, framerate=15/1, width=320, height=280\" ! x264enc key-int-max=30 intra-refresh=true ! rtph264pay name=pay0 pt=96 )");

	return true;
}

bool CameraHandler::ConfigSave(Json::Value &json)
{
	LogDebug("CameraHandler::ConfigSave");

	json["Version"] = Version::ToString();
	
	if (m_Platform->ConfigSave(json["platform"]) == false)
		return false;
	
	if (m_RServer->ConfigSave(json["rtspserver"]) == false)
		return false;

	return true;
}

int CameraHandler::RTSPGetClientCount(int *value)
{
	*value = m_RServer->SessionsCount();
	return 0;
}

int CameraHandler::RTSPSetMaxClients(int max)
{
	m_RServer->SessionsSetMax(max);
	m_Config->Dirty();
	return 0;
}

int CameraHandler::RTSPGetMaxClients(int *max)
{
	*max = m_RServer->SessionsGetMax();
	return 0;
}

int CameraHandler::RTSPSetMaxBacklog(int max)
{
	m_RServer->BacklogSet(max);
	m_Config->Dirty();
	return 0;
}

int CameraHandler::RTSPGetMaxBacklog(int *max)
{
	*max = m_RServer->BacklogGet();
	return 0;
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
	m_Config->Dirty();
	m_QuitBarrier.WakeUp();
}

void CameraHandler::OnPreNewConnection()
{
	LogDebug("CameraHandler::OnPreNewConnection");
}

void CameraHandler::OnPostNewConnection(IServerConnection *Connection)
{
	LogDebug("CameraHandler::OnPostNewConnection");
}

void CameraHandler::OnDisconnect(IServerConnection *Connection)
{
	LogDebug("CameraHandler::OnDisconnect");
}
		
int CameraHandler::OnRequest(IServerConnection *Connection, Request *request, Request *response)
{
	LogDebug("CameraHandler::OnRequest");

	std::string Command = request->GetCommand();

	if (Command == "RTSPGetClientCount")
	{
		int value = 0;
		int ret = RTSPGetClientCount(&value);
		if (ret < 0)
		{
			LogError("RTSPGetClientCount Failed: %d", ret);
			return ret;
		}
		response->SetArg("value", value);
		LogDebug("RTSPGetClientCount { value = %d }", value);
		return 0;
	}

	if (Command == "RTSPSetMaxClients")
	{
		int value = 0;
		if (request->GetInt("value", &value) == false)
		{
			LogError("RTSPSetMaxClients Failed - exists: %s value: %d", request->HasArg("value") ? "true" : "false", value);
			return -EINVAL;
		}
		if (value <= 0)
		{
			LogError("RTSPSetMaxClients Failed - value <= 0 value: %d", value);
			return -EINVAL;
		}
		return RTSPSetMaxClients(value);
	}

	if (Command == "RTSPGetMaxClients")
	{
		int value = 0;
		int ret = RTSPGetMaxClients(&value);
		if (ret < 0)
		{
			LogError("RTSPGetMaxClients Failed: %d", ret);
			return ret;
		}
		response->SetArg("value", value);
		LogDebug("RTSPGetMaxClients { value = %d }", value);
		return 0;
	}

	if (Command == "RTSPSetMaxBacklog")
	{
		int value = 0;
		if (request->GetInt("value", &value) == false)
		{
			LogError("RTSPSetMaxBacklog Failed - exists: %s value: %d", request->HasArg("value") ? "true" : "false", value);
			return -EINVAL;
		}
		if (value <= 0)
		{
			LogError("RTSPSetMaxBacklog Failed - value <= 0 value: %d", value);
			return -EINVAL;
		}
		return RTSPSetMaxBacklog(value);
	}

	if (Command == "RTSPGetMaxBacklog")
	{
		int value = 0;
		int ret = RTSPGetMaxBacklog(&value);
		if (ret < 0)
		{
			LogError("RTSPGetMaxBacklog Failed: %d", ret);
			return ret;
		}
		response->SetArg("value", value);
		LogDebug("RTSPGetMaxBacklog { value = %d }", value);
		return 0;
	}

	if (Command == "VERSION")
	{
		LogDebug("CameraHandler::OnRequest VERSION");
		response->SetArg("value", Version::ToString());
		return 0;
	}

	if (Command == "PING")
	{
		LogDebug("CameraHandler::OnRequest PING");
		return 0;
	}

	if (Command == "QUIT")
	{
		LogDebug("CameraHandler::OnRequest QUIT");
		Quit();
		return 0;
	}

	LogDebug("CameraHandler::OnRequest Unknown Command: \"%s\"", Command.c_str());

	return -ENOSYS;
}

int CameraHandler::OnCommand(IServerConnection *Connection, Request *request)
{
	LogDebug("CameraHandler::OnCommand");
	return -ENOSYS;
}

void CameraHandler::OnBadLine(IServerConnection *Connection, const std::string *line)
{
	LogError("CameraHandler::OnBadLine(%s)", line->c_str());
}



