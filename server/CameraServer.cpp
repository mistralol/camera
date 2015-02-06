
#include <Camera.h>

#include "CameraServerFunctions.i"

CameraServer::CameraServer()
{

}

CameraServer::~CameraServer()
{
	delete m_handler;
}

void CameraServer::Init(const std::string Platform, const std::string CfgFile)
{
	m_handler = new CameraHandler();
	m_handler->Init(Platform, CfgFile);
}

void CameraServer::Wait()
{
	m_handler->Wait();
}

int CameraServer::RTSPGetPort(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	int value = handler->RServer->GetPort();
	response->SetArg("value", value);
	LogDebug("CameraServer::RTSPGetPort { value = %d }", value);
	return 0;
}

int CameraServer::RTSPSetPort(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	int value = 0;
	if (request->GetInt("value", &value) == false)
	{
		LogError("CameraServer:RTSPSetPort Failed - exists: %s value: %d", request->HasArg("value") ? "true" : "false", value);
		return -EINVAL;
	}
	if (value <= 0 || value > 65535)
	{
		LogError("CameraServer:RTSPSetPort Failed - value <= 0 value: %d", value);
		return -EINVAL;
	}
	if (handler->RServer->SetPort(value) < 0)
		return -1;
	handler->Cfg->Dirty();
	return 0;
}

int CameraServer::RTSPGetClientCount(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	int value = handler->RServer->SessionsCount();
	response->SetArg("value", value);
	LogDebug("CameraServer::RTSPGetClientCount { value = %d }", value);
	return 0;
}

int CameraServer::RTSPSetMaxClients(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	int value = 0;
	if (request->GetInt("value", &value) == false)
	{
		LogError("CameraServer:RTSPSetMaxClients Failed - exists: %s value: %d", request->HasArg("value") ? "true" : "false", value);
		return -EINVAL;
	}
	if (value <= 0)
	{
		LogError("CameraServer:RTSPSetMaxClients Failed - value <= 0 value: %d", value);
		return -EINVAL;
	}
	handler->RServer->SessionsSetMax(value);
	handler->Cfg->Dirty();
	return 0;
}

int CameraServer::RTSPGetMaxClients(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	int value = handler->RServer->SessionsGetMax();
	response->SetArg("value", value);
	LogDebug("CameraServer::RTSPGetMaxClients { value = %d }", value);
	return 0;
}

int CameraServer::RTSPSetMaxBacklog(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	int value = 0;
	if (request->GetInt("value", &value) == false)
	{
		LogError("CameraServer::RTSPSetMaxBacklog Failed - exists: %s value: %d", request->HasArg("value") ? "true" : "false", value);
		return -EINVAL;
	}
	if (value <= 0)
	{
		LogError("CameraServer::RTSPSetMaxBacklog Failed - value <= 0 value: %d", value);
		return -EINVAL;
	}
	handler->RServer->BacklogSet(value);
	handler->Cfg->Dirty();
	return 0;
}

int CameraServer::RTSPGetMaxBacklog(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	int value = handler->RServer->BacklogGet();
	response->SetArg("value", value);
	LogDebug("CameraServer::RTSPGetMaxBacklog { value = %d }", value);
	return 0;
}


int CameraServer::Version(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	LogDebug("CameraServer::Version");
	response->SetArg("value", Version::ToString());
	return 0;
}

int CameraServer::Ping(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	LogDebug("CameraServer::OnRequest PING");
	return 0;
}

int CameraServer::Quit(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	LogDebug("CameraServer::OnRequest QUIT");
	handler->Quit();
	return 0;
}

void CameraServer::OnPreNewConnection()
{
	LogDebug("CameraServer::OnPreNewConnection");
}

void CameraServer::OnPostNewConnection(IServerConnection *Connection)
{
	LogDebug("CameraServer::OnPostNewConnection");
}

void CameraServer::OnDisconnect(IServerConnection *Connection)
{
	LogDebug("CameraServer::OnDisconnect");
}
		
int CameraServer::OnRequest(IServerConnection *Connection, Request *request, Request *response)
{
	std::string Command = request->GetCommand();
	const TType *tmp = CameraServerFunctions::Lookup(Command.c_str(), Command.length());
	if (tmp == NULL)
	{
		LogDebug("CameraServer::OnRequest Unknown Command: \"%s\"", Command.c_str());
		return -ENOSYS;
	}
	return tmp->func(m_handler, Connection, request, response);
}

int CameraServer::OnCommand(IServerConnection *Connection, Request *request)
{
	LogDebug("CameraServer::OnCommand");
	return -ENOSYS;
}

void CameraServer::OnBadLine(IServerConnection *Connection, const std::string *line)
{
	LogError("CameraServer::OnBadLine(%s)", line->c_str());
}



