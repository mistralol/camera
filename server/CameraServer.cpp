
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

void CameraServer::Quit()
{
	LogDebug("CameraServer::Quit QUIT");
	m_handler->Quit();
}

int CameraServer::RTSPKickAll(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	handler->RServer->KickAll();
	return 0;
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
		LogError("CameraServer:RTSPSetPort Failed - Paramater '%s' is missing", request->HasArg("value") ? "true" : "false");
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
		LogError("CameraServer:RTSPSetMaxClients Failed - Paramater '%s' is missing", request->HasArg("value") ? "true" : "false");
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
		LogError("CameraServer::RTSPSetMaxBacklog Failed - Paramater '%s' is missing", request->HasArg("value") ? "true" : "false");
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

int CameraServer::VideoStreamCount(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	int count = 0;
	handler->VideoStreamCount(&count);
	response->SetArg("value", count);
	return 0;
}

int CameraServer::VideoStreamSetEnabled(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	unsigned int stream = -1;
	bool enabled = false;

	if (request->GetUInt("stream", &stream) == false)
	{
		LogError("CameraServer::VideoStreamSetEnabled Failed - Paramater '%s' is missing", request->HasArg("value") ? "true" : "false");
		return -EINVAL;
	}

	if (request->GetBool("enabled", &enabled) == false)
	{
		LogError("CameraServer::VideoStreamSetEnabled Failed - Paramater '%s' is missing", request->HasArg("value") ? "true" : "false");
		return -EINVAL;
	}

	if (handler->VideoStreamSetEnabled(stream, enabled))
	{
		handler->Cfg->Dirty();
		return 0;
	}

	return -1;
}

int CameraServer::VideoStreamGetEnabled(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	unsigned int stream = -1;
	bool enabled = false;

	if (request->GetUInt("stream", &stream) == false)
	{
		LogError("CameraServer::VideoStreamGetEnabled Failed - exists: %s", request->HasArg("value") ? "true" : "false");
		return -EINVAL;
	}
	if (handler->VideoStreamGetEnabled(stream, enabled) == false)
	{
		LogError("CameraServer::VideoStreamGetEnabled(%u, *) - Failed", stream);
		return -EINVAL;
	}
	response->SetArg("enabled", enabled);

	return 0;
}

int CameraServer::UserCreate(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string Username = "";
	std::string Password = "";

	if (request->GetString("Username", &Username) == false)
	{
		LogError("CameraServer::UserCreate Failed - Username exists: %s", request->HasArg("Username") ? "true" : "false");
		return -EINVAL;
	}

	if (request->GetString("Password", &Password) == false)
	{
		LogError("CameraServer::UserCreate Failed - Password exists: %s", request->HasArg("Password") ? "true" : "false");
		return -EINVAL;
	}

	int ret = User::Create(Username, Password);
	if (ret >= 0)
		handler->Cfg->Dirty();
	return ret;
}

int CameraServer::UserAuth(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string Username = "";
	std::string Password = "";

	if (request->GetString("Username", &Username) == false)
	{
		LogError("CameraServer::UserAuth Failed - Username exists: %s", request->HasArg("Username") ? "true" : "false");
		return -EINVAL;
	}

	if (request->GetString("Password", &Password) == false)
	{
		LogError("CameraServer::UserAuth Failed - Password exists: %s", request->HasArg("Password") ? "true" : "false");
		return -EINVAL;
	}

	int ret = User::Auth(Username, Password);
	if (ret >= 0)
		handler->Cfg->Dirty();
	return ret;
}

int CameraServer::UserDelete(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string Username = "";

	if (request->GetString("Username", &Username) == false)
	{
		LogError("CameraServer::UserDelete Failed - Username exists: %s", request->HasArg("Username") ? "true" : "false");
		return -EINVAL;
	}

	int ret = User::Delete(Username);
	if (ret >= 0)
		handler->Cfg->Dirty();
	return ret;
}

int CameraServer::UserExists(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string Username = "";

	if (request->GetString("Username", &Username) == false)
	{
		LogError("CameraServer::UserExists Failed - Username exists: %s", request->HasArg("Username") ? "true" : "false");
		return -EINVAL;
	}

	return User::Exists(Username);
}

int CameraServer::UserSetPassword(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string Username = "";
	std::string Password = "";

	if (request->GetString("Username", &Username) == false)
	{
		LogError("CameraServer::UserSetPassword Failed - Username exists: %s", request->HasArg("Username") ? "true" : "false");
		return -EINVAL;
	}

	if (request->GetString("Password", &Password) == false)
	{
		LogError("CameraServer::UserSetPassword Failed - Password exists: %s", request->HasArg("Password") ? "true" : "false");
		return -EINVAL;
	}

	int ret = User::SetPassword(Username, Password);
	if (ret >= 0)
		handler->Cfg->Dirty();
	return ret;
}

int CameraServer::UserList(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::list<std::string> lst = User::List();
	response->SetArg("value", &lst);
	return lst.size();
}

int CameraServer::GroupCreate(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string Group = "";

	if (request->GetString("Group", &Group) == false)
	{
		LogError("CameraServer::GroupCreate Failed - Group exists: %s", request->HasArg("Group") ? "true" : "false");
		return -EINVAL;
	}

	int ret = Group::Create(Group);
	if (ret >= 0)
		handler->Cfg->Dirty();
	return ret;
}

int CameraServer::GroupDelete(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string Group = "";

	if (request->GetString("Group", &Group) == false)
	{
		LogError("CameraServer::GroupDelete Failed - Group exists: %s", request->HasArg("Group") ? "true" : "false");
		return -EINVAL;
	}

	int ret = Group::Delete(Group);
	if (ret >= 0)
		handler->Cfg->Dirty();
	return ret;
}

int CameraServer::GroupExists(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string Group = "";

	if (request->GetString("Group", &Group) == false)
	{
		LogError("CameraServer::GroupExists Failed - Group exists: %s", request->HasArg("Group") ? "true" : "false");
		return -EINVAL;
	}

	return Group::Exists(Group);
}

int CameraServer::GroupIsUserInGroup(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string Group = "";
	std::string Username = "";

	if (request->GetString("Group", &Group) == false)
	{
		LogError("CameraServer::GroupIsUserInGroup Failed - Group exists: %s", request->HasArg("Group") ? "true" : "false");
		return -EINVAL;
	}

	if (request->GetString("Username", &Username) == false)
	{
		LogError("CameraServer::GroupIsUserInGroup Failed - Username exists: %s", request->HasArg("Username") ? "true" : "false");
		return -EINVAL;
	}

	return Group::IsUserInGroup(Group, Username);
}

int CameraServer::GroupUserAdd(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string Group = "";
	std::string Username = "";

	if (request->GetString("Group", &Group) == false)
	{
		LogError("CameraServer::GroupIsUserInGroup Failed - Group exists: %s", request->HasArg("Group") ? "true" : "false");
		return -EINVAL;
	}

	if (request->GetString("Username", &Username) == false)
	{
		LogError("CameraServer::GroupIsUserInGroup Failed - Username exists: %s", request->HasArg("Username") ? "true" : "false");
		return -EINVAL;
	}

	int ret = Group::UserAdd(Group, Username);
	if (ret >= 0)
		handler->Cfg->Dirty();
	return ret;
}

int CameraServer::GroupUserRemove(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string Group = "";
	std::string Username = "";

	if (request->GetString("Group", &Group) == false)
	{
		LogError("CameraServer::GroupIsUserInGroup Failed - Group exists: %s", request->HasArg("Group") ? "true" : "false");
		return -EINVAL;
	}

	if (request->GetString("Username", &Username) == false)
	{
		LogError("CameraServer::GroupIsUserInGroup Failed - Username exists: %s", request->HasArg("Username") ? "true" : "false");
		return -EINVAL;
	}

	int ret = Group::UserRemove(Group, Username);
	if (ret >= 0)
		handler->Cfg->Dirty();
	return ret;
}

int CameraServer::GroupList(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::list<std::string> lst = Group::List();
	response->SetArg("value", &lst);
	return lst.size();
}

int CameraServer::GroupListUsers(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string Group = "";

	if (request->GetString("Group", &Group) == false)
	{
		LogError("CameraServer::GroupIsUserInGroup Failed - Group exists: %s", request->HasArg("Group") ? "true" : "false");
		return -EINVAL;
	}

	std::list<std::string> lst = Group::List();
	response->SetArg("value", &lst);
	return lst.size();
}

int CameraServer::WebServerGetPort(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	int port = handler->WServer->GetPort();
	response->SetArg("port", port);
	return 0;
}

int CameraServer::WebServerSetPort(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	int port = -1;

	if (request->GetInt("port", &port) == false)
	{
		LogError("CameraServer::WebServerSetPort Failed - Paramater '%s' is missing", request->HasArg("port") ? "true" : "false");
		return -EINVAL;
	}

	int ret = handler->WServer->SetPort(port);
	if (ret <= 0)
		return ret;
	handler->Cfg->Dirty();
	return ret;
}

int CameraServer::WebServerGetEnabled(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	bool enabled = handler->WServer->GetEnabled();
	response->SetArg("enabled", enabled);
	return 0;
}

int CameraServer::WebServerSetEnabled(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	bool enabled = false;

	if (request->GetBool("enabled", &enabled) == false)
	{
		LogError("CameraServer::WebServerSetEnabled Failed - Paramater '%s' is missing", request->HasArg("value") ? "true" : "false");
		return -EINVAL;
	}

	handler->WServer->SetEnabled(enabled);
	handler->Cfg->Dirty();
	return 0;
}

int CameraServer::WebServerRestart(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	handler->WServer->Restart();
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
	LogDebug("CameraServer::Ping PING");
	return 0;
}

int CameraServer::Quit(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	LogDebug("CameraServer::Quit QUIT");
	handler->Quit();
	return 0;
}

void CameraServer::StatsDump()
{
	std::list<std::string> Keys = PerfManager::GetKeyList();
	struct PerfDetails_t info;

	LogInfo("Dumping Stats");
	for(std::list<std::string>::iterator it = Keys.begin(); it != Keys.end(); it++)
	{
		if (PerfManager::GetInfo(*it, &info) < 0)
			continue;
		if (info.Count == 0)
			continue;
		LogInfo("%15s Total Calls: %d Average Time: %8ld.%ld Seconds",
				it->c_str(), info.Count,
				info.TotalTime.tv_sec / info.Count,
				info.TotalTime.tv_nsec / 1000 / info.Count);
	}
	LogInfo("Finished Dumping stats");
}

void CameraServer::StatsReset()
{
	LogInfo("Resetting Stats");
	PerfManager::Clear();
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
	PerfCounter PC(Command.c_str());
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



