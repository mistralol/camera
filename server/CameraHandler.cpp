
#include <main.h>

CameraHandler::CameraHandler()
{
	LogDebug("RTSPHandler::RTSPHandler");
}

CameraHandler::~CameraHandler()
{
	LogDebug("RTSPHandler::~RTSPHandler");
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

void CameraHandler::Wait()
{
	LogDebug("RTSPHandler::Wait");
	m_QuitBarrier.Wait();
}

void CameraHandler::Quit()
{
	LogDebug("RTSPHandler::Quit");
	m_QuitBarrier.WakeUp();
}

