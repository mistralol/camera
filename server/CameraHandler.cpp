
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

	m_Config = new Config(this, CfgFile);

	if (m_Config->Load() == false)
	{
		LogCritical("Failed To Load Config File: \"%s\" exiting ....", m_CfgFile.c_str());
		exit(EXIT_FAILURE);
	}
}

bool CameraHandler::ConfigLoad(Json::Value *json)
{
	LogDebug("CameraHandler::ConfigLoad");

	//FIXME: Pass JSon Struct to correct places for loading the config

	//Temporary the Platform Should do this
	m_RServer->PipelineAdd("/test", "( videotestsrc horizontal-speed=5 is-live=true ! capsfilter caps=capsfilter caps=\"video/x-raw, framerate=15/1, width=320, height=280\" ! x264enc key-int-max=30 intra-refresh=true ! rtph264pay name=pay0 pt=96 )");

	return true;
}

bool CameraHandler::ConfigSave(Json::Value *json)
{
	LogDebug("CameraHandler::ConfigSave");
	
	//Save All Settings

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



