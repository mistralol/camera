
#include <Camera.h>

#include "CameraServerFunctions.i"

CameraServer::CameraServer()
{

}

CameraServer::~CameraServer()
{
	delete m_handler;
}

void CameraServer::Init(const std::string WebRoot, const std::string Platform, const std::string CfgFile)
{
	m_handler = new CameraHandler();
	m_handler->Init(WebRoot, Platform, CfgFile);
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
	if (value < 0)
	{
		LogError("CameraServer:RTSPSetMaxClients Failed - value < 0 value: %d", value);
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

int CameraServer::VideoInputCount(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	int count = handler->VideoInputCount();
	response->SetArg("value", count);
	return 0;
}

int CameraServer::VideoInputSetEnabled(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	unsigned int input = -1;
	bool enabled = false;

	if (request->GetUInt("input", &input) == false)
	{
		LogError("CameraServer::VideoInputSetEnabled Failed - Paramater '%s' is missing", request->HasArg("value") ? "true" : "false");
		return -EINVAL;
	}

	if (request->GetBool("enabled", &enabled) == false)
	{
		LogError("CameraServer::VideoInputSetEnabled Failed - Paramater '%s' is missing", request->HasArg("value") ? "true" : "false");
		return -EINVAL;
	}

	if (handler->VideoInputSetEnabled(input, enabled))
	{
		handler->Cfg->Dirty();
		return 0;
	}

	return -1;
}

int CameraServer::VideoInputGetEnabled(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	unsigned int input = -1;
	bool enabled = false;

	if (request->GetUInt("input", &input) == false)
	{
		LogError("CameraServer::VideoInputGetEnabled Failed - exists: %s", request->HasArg("value") ? "true" : "false");
		return -EINVAL;
	}
	if (handler->VideoInputGetEnabled(input, enabled) == false)
	{
		LogError("CameraServer::VideoInputGGetEnabled(%u, *) - Failed", input);
		return -EINVAL;
	}
	response->SetArg("enabled", enabled);

	return 0;
}

int CameraServer::VideoInputGetConfig(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	unsigned int input = -1;
	VideoInputConfig cfg;

	if (request->GetUInt("input", &input) == false)
	{
		LogError("CameraServer::VideoInputGetConfig Failed - exists: %s", request->HasArg("value") ? "true" : "false");
		return -EINVAL;
	}

	int ret = handler->VideoInputGetConfig(input, &cfg);
	if (ret < 0)
		return ret;
	
	response->SetArg("config", cfg.Encode());
	return ret;
}

int CameraServer::VideoInputSetConfig(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	unsigned int input = -1;
	VideoInputConfig cfg;
	std::string data;
	
	if (request->GetUInt("input", &input) == false)
	{
		LogError("CameraServer::VideoInputSetConfig Failed - exists: %s", request->HasArg("value") ? "true" : "false");
		return -EINVAL;
	}
	
	if (request->GetString("config", &data) == false)
	{
		LogError("CameraServer::VideoInputSetConfig Failed - exists: %s", request->HasArg("value") ? "true" : "false");
		return -EINVAL;
	}
	
	if (cfg.Decode(data) == false)
	{
		LogError("CameraServer::VideoInputSetConfig Failed to decode config - exists: %s", request->HasArg("value") ? "true" : "false");
		return -EINVAL;
	}
	
	int ret = handler->VideoInputSetConfig(input, &cfg);
	handler->Cfg->Dirty();
	return ret;
}

int CameraServer::VideoInputGetSupported(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	unsigned int input = -1;
	VideoInputSupported info;

	if (request->GetUInt("input", &input) == false)
	{
		LogError("CameraServer::VideoInputGetSupported Failed - exists: %s", request->HasArg("value") ? "true" : "false");
		return -EINVAL;
	}

	int ret = handler->VideoInputGetSupported(input, &info);
	if (ret < 0)
		return ret;
	
	response->SetArg("info", info.Encode());
	return ret;
}

int CameraServer::GPIOOutputCount(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	int count = handler->GPIOOutputCount();
	response->SetArg("value", count);
	return 0;
}

int CameraServer::GPIOOutputSetState(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	unsigned int output = -1;
	bool enabled = false;
	struct timespec timeout = { 0, 0};
	
	if (request->GetUInt("output", &output) == false)
	{
		LogError("CameraServer::GPIOOutputSetState Failed - exists: %s", request->HasArg("output") ? "true" : "false");
		return -EINVAL;
	}

	if (request->GetBool("value", &enabled) == false)
	{
		LogError("CameraServer::GPIOOutputSetState Failed - exists: %s", request->HasArg("value") ? "true" : "false");
		return -EINVAL;
	}

	if (request->HasArg("timeout"))
	{
		if (request->GetTimeSpec("timeout", &timeout) == false)
		{
			LogError("CameraServer::GPIOOutputSetState Failed - exists: %s", request->HasArg("timeout") ? "true" : "false");
			return -EINVAL;
		}
		return handler->GPIOOutputSetState(output, enabled, &timeout);
	}
	else
	{
		return handler->GPIOOutputSetState(output, enabled);
	}
	
	abort(); //Unreachable
	return -1;
}

int CameraServer::GPIOOutputGetState(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	unsigned int output = -1;
	bool enabled = false;
	
	if (request->GetUInt("output", &output) == false)
	{
		LogError("CameraServer::GPIOOutputGetState Failed - exists: %s", request->HasArg("output") ? "true" : "false");
		return -EINVAL;
	}
	enabled = handler->GPIOOutputGetState(output);
	response->SetArg("enabled", enabled);

	return 0;
}

int CameraServer::UserCreate(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string Username = "";
	std::string Password = "";
	std::string EMail = "";

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
	
	if (request->GetString("EMail", &EMail) == false)
	{
		LogError("CameraServer::UserCreate Failed - EMail exists: %s", request->HasArg("Password") ? "true" : "false");
		return -EINVAL;
	}

	int ret = User::Create(Username, Password, EMail);
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

int CameraServer::UserTouch(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string Username = "";

	if (request->GetString("Username", &Username) == false)
	{
		LogError("CameraServer::UserTouch Failed - Username exists: %s", request->HasArg("Username") ? "true" : "false");
		return -EINVAL;
	}
	return User::Touch(Username);
}

int CameraServer::UserIsLockedOut(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string Username = "";

	if (request->GetString("Username", &Username) == false)
	{
		LogError("CameraServer::UserIsLockedOut Failed - Username exists: %s", request->HasArg("Username") ? "true" : "false");
		return -EINVAL;
	}
	return User::IsLockedOut(Username);
}

int CameraServer::UserIsApproved(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string Username = "";

	if (request->GetString("Username", &Username) == false)
	{
		LogError("CameraServer::UserIsApproved Failed - Username exists: %s", request->HasArg("Username") ? "true" : "false");
		return -EINVAL;
	}
	return User::IsApproved(Username);
}

int CameraServer::UserIsOnline(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string Username = "";

	if (request->GetString("Username", &Username) == false)
	{
		LogError("CameraServer::UserIsOnline Failed - Username exists: %s", request->HasArg("Username") ? "true" : "false");
		return -EINVAL;
	}
	return User::IsOnline(Username);
}

int CameraServer::UserSetLockedOut(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string Username = "";
	bool value = false;

	if (request->GetString("Username", &Username) == false)
	{
		LogError("CameraServer::UserSetLockedOut Failed - Username exists: %s", request->HasArg("Username") ? "true" : "false");
		return -EINVAL;
	}
	
	if (request->GetBool("value", &value) == false)
	{
		LogError("CameraServer::UserSetLockedOut Failed - value exists: %s", request->HasArg("Username") ? "true" : "false");
		return -EINVAL;
	}
	int ret = User::SetLockedOut(Username, value);
	handler->Cfg->Dirty();
	return ret;
}

int CameraServer::UserSetApproved(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string Username = "";
	bool value = false;

	if (request->GetString("Username", &Username) == false)
	{
		LogError("CameraServer::UserSetApproved Failed - Username exists: %s", request->HasArg("Username") ? "true" : "false");
		return -EINVAL;
	}
	
	if (request->GetBool("value", &value) == false)
	{
		LogError("CameraServer::UserSetApproved Failed - value exists: %s", request->HasArg("Username") ? "true" : "false");
		return -EINVAL;
	}
	int ret = User::SetApproved(Username, value);
	handler->Cfg->Dirty();
	return ret;
}

int CameraServer::UserGetLockoutDuration(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	return User::GetLockoutDuration();
}

int CameraServer::UserSetLockoutDuration(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	int value = 0;
	if (request->GetInt("value", &value) == false)
	{
		LogError("CameraServer::UserSetLockoutDuration Failed - value exists: %s", request->HasArg("value") ? "true" : "false");
		return -EINVAL;
	}
	int ret = User::SetLockoutDuration(value);
	handler->Cfg->Dirty();
	return ret;
}

int CameraServer::UserGetMaxFailedAttempts(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	return User::GetMaxFailedAttempts();
}

int CameraServer::UserSetMaxFailedAttempts(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	int value = 0;
	if (request->GetInt("value", &value) == false)
	{
		LogError("CameraServer::UserSetMaxFailedAttempts Failed - value exists: %s", request->HasArg("value") ? "true" : "false");
		return -EINVAL;
	}
	int ret = User::SetMaxFailedAttempts(value);
	handler->Cfg->Dirty();
	return ret;
}

int CameraServer::UserGetAutoLogOff(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	return User::GetAutoLogOff();
}

int CameraServer::UserSetAutoLogOff(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	int value = 0;
	if (request->GetInt("value", &value) == false)
	{
		LogError("CameraServer::UserSetAutoLogOff Failed - value exists: %s", request->HasArg("value") ? "true" : "false");
		return -EINVAL;
	}
	int ret = User::SetAutoLogOff(value);
	handler->Cfg->Dirty();
	return ret;
}

int CameraServer::UserInfo(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string Username = "";
	struct UserItem info;
	if (request->GetString("Username", &Username) == false)
	{
		LogError("CameraServer::UserSetApproved Failed - Username exists: %s", request->HasArg("Username") ? "true" : "false");
		return -EINVAL;
	}
	int ret = User::UserInfo(Username, &info);
	if (ret < 0)
		return ret;
	response->SetArg("info", info.Encode());
	return 0;
}

int CameraServer::UserList(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::vector<std::string> lst = User::List();
	response->SetArg("value", &lst);
	return lst.size();
}

int CameraServer::UserGetFromEMail(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string EMail = "";
	std::string Username = "";
	struct UserItem info;
	if (request->GetString("EMail", &EMail) == false)
	{
		LogError("CameraServer::UserGetFromEMail Failed - Key exists: %s", request->HasArg("Username") ? "true" : "false");
		return -EINVAL;
	}
	int ret = User::GetUserFromEMail(EMail, &Username);
	if (ret < 0)
		return ret;
	response->SetArg("Username", Username);
	return ret;
}

int CameraServer::UserGetFromKey(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string Key = "";
	std::string Username = "";
	struct UserItem info;
	if (request->GetString("Key", &Key) == false)
	{
		LogError("CameraServer::UserGetFromKey Failed - Key exists: %s", request->HasArg("Username") ? "true" : "false");
		return -EINVAL;
	}
	int ret = User::GetUserFromKey(Key, &Username);
	if (ret < 0)
		return ret;
	response->SetArg("Username", Username);
	return ret;
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

int CameraServer::WebServerGetProperty(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string key = "";
	std::string value = "";
	std::string def = ""; //Optional
	
	if (request->GetString("key", &key) == false)
	{
		LogError("CameraServer::WebServerGetProperty - Missing argument 'key': %s", request->HasArg("key") ? "true" : "false");
		return -EINVAL;
	}
	
	if (request->GetString("def", &def) == true)
	{
		value = handler->WServer->GetProperty(key, def);
	}
	else
	{
		value = handler->WServer->GetProperty(key);
	}

	response->SetArg("value", value);
	return 0;
}

int CameraServer::WebServerSetProperty(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string key = "";
	std::string value = "";
	
	if (request->GetString("key", &key) == false)
	{
		LogError("CameraServer::WebServerSetProperty - Missing argument 'key': %s", request->HasArg("key") ? "true" : "false");
		return -EINVAL;
	}
	
	if (request->GetString("value", &value) == false)
	{
		LogError("CameraServer::WebServerSetProperty - Missing argument 'value': %s", request->HasArg("value") ? "true" : "false");
		return -EINVAL;
	}
	handler->WServer->SetProperty(key, value);
	handler->Cfg->Dirty();
	return 0;
}


int CameraServer::WebServerRestart(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	handler->WServer->Restart();
	return 0;
}

int CameraServer::WebStreamStart(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string str;
	WebStreamOptions options;

	if (request->GetString("options", &str) == false)
	{
		LogError("CameraServer::WebServerRestart Failed - exists: %s", request->HasArg("value") ? "true" : "false");
		return -EINVAL;
	}
	if (options.Decode(str) == false)
	{
		LogError("CameraServer::WebStreamStart - Faield to decode options");
		return -EINVAL;
	}

	return handler->WStream->StartVideoInput(&options);
}

int CameraServer::DebugGetEnabled(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	bool enabled = Debug::GetEnabled();
	response->SetArg("enabled", enabled);
	return 0;
}

int CameraServer::DebugSetEnabled(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	bool enabled = false;

	if (request->GetBool("enabled", &enabled) == false)
	{
		LogError("CameraServer::WebServerSetEnabled Failed - Paramater '%s' is missing", request->HasArg("value") ? "true" : "false");
		return -EINVAL;
	}

	Debug::SetEnabled(enabled);
	handler->Cfg->Dirty();
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

int CameraServer::Log(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	std::string Level;
	std::string Message;
	if (request->GetString("Level", &Level) == false)
	{
		LogError("CameraServer::Log - Missing argument 'Level': %s", request->HasArg("Level") ? "true" : "false");
		return -EINVAL;
	}
	
	if (request->GetString("Message", &Message) == false)
	{
		LogError("CameraServer::Log - Missing argument 'Message': %s", request->HasArg("Message") ? "true" : "false");
		return -EINVAL;
	}
	
	if (Level == "DEBUG")
	{
		LogDebug("%s", Message.c_str());
		return 0;
	}
	
	if (Level == "INFO")
	{
		LogInfo("%s", Message.c_str());
		return 0;
	}
	
	if (Level == "NOTICE")
	{
		LogNotice("%s", Message.c_str());
		return 0;
	}
	
	if (Level == "WARNING")
	{
		LogWarning("%s", Message.c_str());
		return 0;
	}
	
	if (Level == "ERROR")
	{
		LogError("%s", Message.c_str());
		return 0;
	}
	
	if (Level == "CRITICAL")
	{
		LogCritical("%s", Message.c_str());
		return 0;
	}
	
	if (Level == "ALERT")
	{
		LogAlert("%s", Message.c_str());
		return 0;
	}
	
	if (Level == "EMERGENCY")
	{
		LogEmergency("%s", Message.c_str());
		return 0;
	}
	
	LogError("CameraServer::Log - Unknown Log Level: '%s' Message: %s", Level.c_str(), Message.c_str());
	return -EINVAL;
}

int CameraServer::StatsList(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	LogDebug("CameraServer::StatsList");
	std::list<std::string> Keys = PerfManager::GetKeyList();
	response->SetArg("list", &Keys);
	return Keys.size();
}

int CameraServer::StatsInfo(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	LogDebug("CameraServer::StatsInfo");
	std::string Key = "";
	struct PerfDetails_t info;
	
	if (request->GetString("key", &Key) == false)
	{
		LogError("CameraServer::StatsInfo - Missing argument 'key': %s", request->HasArg("key") ? "true" : "false");
		return -EINVAL;
	}
	
	if (PerfManager::GetInfo(Key, &info) < 0)
		return -ENOLINK;
		
	response->SetArg("TotalTime", (int) Time::Microseconds(&info.TotalTime));
	response->SetArg("WorstTime", (int) Time::Microseconds(&info.WorstTime));
	response->SetArg("BestTime", (int) Time::Microseconds(&info.BestTime));
	response->SetArg("Count", info.Count);
	return 0;
}

int CameraServer::StatsDump(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	LogDebug("CameraServer::StatsDump");
	StatsDump();
}

int CameraServer::StatsReset(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response)
{
	LogDebug("CameraServer::StatsReset");
	StatsReset();
}


void CameraServer::StatsDump()
{
	std::list<std::string> Keys = PerfManager::GetKeyList();
	struct PerfDetails_t info;

	LogInfo("CameraServer::Dumping Stats");
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
	LogInfo("CameraServer::Finished Dumping stats");
}

void CameraServer::StatsReset()
{
	LogInfo("CameraServer::Resetting Stats");
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
	TimerAbort abrt = TimerAbort(120);
	std::string Command = request->GetCommand();
	const TType *tmp = CameraServerFunctions::Lookup(Command.c_str(), Command.length());
	if (tmp == NULL)
	{
		LogDebug("CameraServer::OnRequest Unknown Command: \"%s\"", Command.c_str());
		return -ENOSYS;
	}
	PerfCounter PC(Command.c_str());
	CameraTimers->Add(&abrt);
	int ret = tmp->func(m_handler, Connection, request, response);
	CameraTimers->Remove(&abrt);
	return ret;
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



