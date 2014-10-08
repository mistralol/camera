
#include <main.h>

RTSPHandler::RTSPHandler()
{
	LogDebug("RTSPHandler::RTSPHandler");
}

RTSPHandler::~RTSPHandler()
{
	LogDebug("RTSPHandler::~RTSPHandler");
}

void RTSPHandler::OnPreNewConnection()
{
	LogDebug("RTSPHandler::OnPreNewConnection");
}

void RTSPHandler::OnPostNewConnection(IServerConnection *Connection)
{
	LogDebug("RTSPHandler::OnPostNewConnection");
}

void RTSPHandler::OnDisconnect(IServerConnection *Connection)
{
	LogDebug("RTSPHandler::OnDisconnect");
}
		
int RTSPHandler::OnRequest(IServerConnection *Connection, Request *request, Request *response)
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

int RTSPHandler::OnCommand(IServerConnection *Connection, Request *request)
{
	LogDebug("RTSPHandler::OnCommand");
	return -ENOSYS;
}

void RTSPHandler::OnBadLine(IServerConnection *Connection, const std::string *line)
{
	LogDebug("RTSPHandler::OnBadLine(%s)", line->c_str());
}

void RTSPHandler::Wait()
{
	LogDebug("RTSPHandler::Wait");
	m_QuitBarrier.Wait();
}

void RTSPHandler::Quit()
{
	LogDebug("RTSPHandler::Quit");
	m_QuitBarrier.WakeUp();
}

