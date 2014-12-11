
#include <Camera.h>

CameraHandler::CameraHandler()
{
	LogDebug("CameraHandler::RTSPHandler");
	gst_init (0, NULL);
}

CameraHandler::~CameraHandler()
{
	LogDebug("CameraHandler::~RTSPHandler");
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
		LogCritical("Fail to loaf platform: %s", Platform.c_str());
		exit(EXIT_FAILURE);
	}

	if (m_Platform->Init() == false)
	{
		LogCritical("Failed To Init Platform exiting....");
		exit(EXIT_FAILURE);
	}

	if (ConfigLoad() == false)
	{
		LogCritical("Failed To Load Config File: \"%s\" exiting ....", m_CfgFile.c_str());
		exit(EXIT_FAILURE);
	}
}

bool CameraHandler::ConfigLoad()
{
	LogDebug("CameraHandler::ConfigLoad");

	//Temporary the Platform Should do this
	m_RServer->PipelineAdd("/test", "( videotestsrc horizontal-speed=5 is-live=true ! capsfilter caps=capsfilter caps=\"video/x-raw, framerate=15/1, width=320, height=280\" ! x264enc key-int-max=30 intra-refresh=true ! rtph264pay name=pay0 pt=96 )");

	return true;
}

bool CameraHandler::ConfigSave()
{
	LogDebug("CameraHandler::ConfigSave");

	return false;
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
	LogDebug("RTSPHandler::OnPreNewConnection");
}

void CameraHandler::OnPostNewConnection(IServerConnection *Connection)
{
	LogDebug("RTSPHandler::OnPostNewConnection");
}

void CameraHandler::OnDisconnect(IServerConnection *Connection)
{
	LogDebug("RTSPHandler::OnDisconnect");
}
		
int CameraHandler::OnRequest(IServerConnection *Connection, Request *request, Request *response)
{
	LogDebug("RTSPHandler::OnRequest");

	std::string Command = request->GetCommand();

	if (Command == "QUIT")
	{
		Quit();
		return 0;
	}

	return -ENOSYS;
}

int CameraHandler::OnCommand(IServerConnection *Connection, Request *request)
{
	LogDebug("RTSPHandler::OnCommand");
	return -ENOSYS;
}

void CameraHandler::OnBadLine(IServerConnection *Connection, const std::string *line)
{
	LogDebug("RTSPHandler::OnBadLine(%s)", line->c_str());
}



