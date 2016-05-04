
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

int CameraServer::VideoInputCount(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	int count = handler->VideoInputCount();
	response["value"] = count;
	return 0;
}

int CameraServer::VideoInputSetEnabled(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	unsigned int input = -1;
	bool enabled = false;
/*
	if (request->GetUInt("input", &input) == false)
	{
		LogError("CameraServer::VideoInputSetEnabled Failed - Paramater '%s' is missing", request.isMember("input") ? "true" : "false");
		return -EINVAL;
	}

	if (request->GetBool("enabled", &enabled) == false)
	{
		LogError("CameraServer::VideoInputSetEnabled Failed - Paramater '%s' is missing", request.isMember("enabled") ? "true" : "false");
		return -EINVAL;
	}

	if (handler->VideoInputSetEnabled(input, enabled))
	{
		handler->Cfg->Dirty();
		return 0;
	}
*/
	return -1;
}

int CameraServer::VideoInputGetEnabled(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	unsigned int input = -1;
	bool enabled = false;
/*
	if (request->GetUInt("input", &input) == false)
	{
		LogError("CameraServer::VideoInputGetEnabled Failed - exists: %s", request.isMember("input") ? "true" : "false");
		return -EINVAL;
	}
	if (handler->VideoInputGetEnabled(input, enabled) == false)
	{
		LogError("CameraServer::VideoInputGGetEnabled(%u, *) - Failed", input);
		return -EINVAL;
	}
	response->SetArg("enabled", enabled);
*/
	return -1;
}

int CameraServer::VideoInputGetConfig(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	unsigned int input = -1;
	VideoInputConfig cfg;

/*
	if (request->GetUInt("input", &input) == false)
	{
		LogError("CameraServer::VideoInputGetConfig Failed - exists: %s", request.isMember("input") ? "true" : "false");
		return -EINVAL;
	}

	int ret = handler->VideoInputGetConfig(input, &cfg);
	if (ret < 0)
		return ret;
	
	response->SetArg("config", cfg.Encode());
*/
	return -1;
}

int CameraServer::VideoInputSetConfig(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
/*
	unsigned int input = -1;
	VideoInputConfig cfg;
	std::string data;
	
	if (request->GetUInt("input", &input) == false)
	{
		LogError("CameraServer::VideoInputSetConfig Failed - exists: %s", request.isMember("input") ? "true" : "false");
		return -EINVAL;
	}
	
	if (request->GetString("config", &data) == false)
	{
		LogError("CameraServer::VideoInputSetConfig Failed - exists: %s", request.isMember("config") ? "true" : "false");
		return -EINVAL;
	}
	
	if (cfg.Decode(data) == false)
	{
		LogError("CameraServer::VideoInputSetConfig Failed to decode config - exists: %s", request.isMember("value") ? "true" : "false");
		return -EINVAL;
	}
	
	int ret = handler->VideoInputSetConfig(input, &cfg);
	handler->Cfg->Dirty();
	return ret;
*/
}

int CameraServer::VideoInputGetSupported(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
/*
	if (request.isMember("input") == false || request["input"].isInt() == false)
	{
		LogError("CameraServer::VideoInputGetSupported Failed - exists: %s", request.isMember("input") ? "true" : "false");
		return -EINVAL;
	}

	int input = request["input"].asInt();
	VideoInputSupported info;
	if (input < 0)
		return -EINVAL;


	int ret = handler->VideoInputGetSupported(input, &info);
	if (ret < 0)
		return ret;
	
	Json::Value json;
	info.Encode(info);
	response["info"] = json;
*/
	return -1;
}

int CameraServer::UserCreate(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	if (request.isMember("Username") == false || request["Username"].isString() == false)
	{
		LogError("CameraServer::UserCreate Failed - Username exists: %s", request.isMember("Username") ? "true" : "false");
		return -EINVAL;
	}

	if (request.isMember("Password") == false || request["Password"].isString() == false)
	{
		LogError("CameraServer::UserCreate Failed - Password exists: %s", request.isMember("Password") ? "true" : "false");
		return -EINVAL;
	}
	
	if (request.isMember("EMail") == false || request["EMail"].isString() == false)
	{
		LogError("CameraServer::UserCreate Failed - EMail exists: %s", request.isMember("Password") ? "true" : "false");
		return -EINVAL;
	}
	
	std::string Username = request["Username"].asString();
	std::string Password = request["Password"].asString();
	std::string EMail = request["EMail"].asString();

	int ret = User::Create(Username, Password, EMail);
	if (ret >= 0)
		handler->Cfg->Dirty();
	return ret;
}

int CameraServer::UserAuth(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	if (request.isMember("Username") == false || request["Username"].isString() == false)
	{
		LogError("CameraServer::UserAuth Failed - Username exists: %s", request.isMember("Username") ? "true" : "false");
		return -EINVAL;
	}

	if (request.isMember("Password") == false || request["Password"].isString() == false)
	{
		LogError("CameraServer::UserAuth Failed - Password exists: %s", request.isMember("Password") ? "true" : "false");
		return -EINVAL;
	}
	
	std::string Username = request["Username"].asString();
	std::string Password = request["Password"].asString();
	int ret = User::Auth(Username, Password);
	if (ret >= 0)
		handler->Cfg->Dirty();
	return ret;
}

int CameraServer::UserDelete(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	if (request.isMember("Username") == false || request["Username"].isString() == false)
	{
		LogError("CameraServer::UserDelete Failed - Username exists: %s", request.isMember("Username") ? "true" : "false");
		return -EINVAL;
	}

	std::string Username = request["Username"].asString();
	int ret = User::Delete(Username);
	if (ret >= 0)
		handler->Cfg->Dirty();
	return ret;
}

int CameraServer::UserExists(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	if (request.isMember("Username") == false || request["Username"].isString() == false)
	{
		LogError("CameraServer::UserExists Failed - Username exists: %s", request.isMember("Username") ? "true" : "false");
		return -EINVAL;
	}

	std::string Username = request["Username"].asString();
	return User::Exists(Username);
}

int CameraServer::UserSetPassword(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	if (request.isMember("Username") == false || request["Username"].isString() == false)
	{
		LogError("CameraServer::UserSetPassword Failed - Username exists: %s", request.isMember("Username") ? "true" : "false");
		return -EINVAL;
	}

	if (request.isMember("Password") == false || request["Password"].isString() == false)
	{
		LogError("CameraServer::UserSetPassword Failed - Password exists: %s", request.isMember("Password") ? "true" : "false");
		return -EINVAL;
	}

	std::string Username = request["Username"].asString();
	std::string Password = request["Password"].asString();
	int ret = User::SetPassword(Username, Password);
	if (ret >= 0)
		handler->Cfg->Dirty();
	return ret;
}

int CameraServer::UserTouch(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	if (request.isMember("Username") == false || request["Username"].isString() == false)
	{
		LogError("CameraServer::UserTouch Failed - Username exists: %s", request.isMember("Username") ? "true" : "false");
		return -EINVAL;
	}
	
	std::string Username = request["Username"].asString();
	return User::Touch(Username);
}

int CameraServer::UserIsLockedOut(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	if (request.isMember("Username") == false || request["Username"].isString() == false)
	{
		LogError("CameraServer::UserIsLockedOut Failed - Username exists: %s", request.isMember("Username") ? "true" : "false");
		return -EINVAL;
	}
	
	std::string Username = request["Username"].asString();
	return User::IsLockedOut(Username);
}

int CameraServer::UserIsApproved(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	if (request.isMember("Username") == false || request["Username"].isString() == false)
	{
		LogError("CameraServer::UserIsApproved Failed - Username exists: %s", request.isMember("Username") ? "true" : "false");
		return -EINVAL;
	}
	
	std::string Username = request["Username"].asString();
	return User::IsApproved(Username);
}

int CameraServer::UserIsOnline(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	if (request.isMember("Username") == false || request["Username"].isString() == false)
	{
		LogError("CameraServer::UserIsOnline Failed - Username exists: %s", request.isMember("Username") ? "true" : "false");
		return -EINVAL;
	}

	std::string Username = request["Username"].asString();
	return User::IsOnline(Username);
}

int CameraServer::UserSetLockedOut(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	if (request.isMember("Username") == false || request["Username"].isString() == false)
	{
		LogError("CameraServer::UserSetLockedOut Failed - Username exists: %s", request.isMember("Username") ? "true" : "false");
		return -EINVAL;
	}

	if (request.isMember("value") == false || request["value"].isBool() == false)
	{
		LogError("CameraServer::UserSetLockedOut Failed - value exists: %s", request.isMember("value") ? "true" : "false");
		return -EINVAL;
	}
	
	std::string Username = request["Username"].asString();
	bool value = request["value"].asBool();
	
	int ret = User::SetLockedOut(Username, value);
	handler->Cfg->Dirty();
	return ret;
}

int CameraServer::UserSetApproved(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	if (request.isMember("Username") == false || request["Username"].isString() == false)
	{
		LogError("CameraServer::UserSetApproved Failed - Username exists: %s", request.isMember("Username") ? "true" : "false");
		return -EINVAL;
	}

	if (request.isMember("value") == false || request["value"].isBool() == false)
	{
		LogError("CameraServer::UserSetApproved Failed - value exists: %s", request.isMember("Username") ? "true" : "false");
		return -EINVAL;
	}
	
	std::string Username = request["Username"].asString();
	bool value = request["value"].asBool();
	
	int ret = User::SetApproved(Username, value);
	handler->Cfg->Dirty();
	return ret;
}

int CameraServer::UserGetLockoutDuration(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	return User::GetLockoutDuration();
}

int CameraServer::UserSetLockoutDuration(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	if (request.isMember("value") == false || request["value"].isInt() == false)
	{
		LogError("CameraServer::UserSetLockoutDuration Failed - value exists: %s", request.isMember("value") ? "true" : "false");
		return -EINVAL;
	}
	int value = request["value"].asInt();
	int ret = User::SetLockoutDuration(value);
	handler->Cfg->Dirty();
	return ret;
}

int CameraServer::UserGetMaxFailedAttempts(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	return User::GetMaxFailedAttempts();
}

int CameraServer::UserSetMaxFailedAttempts(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	if (request.isMember("value") == false || request["value"].isInt() == false)
	{
		LogError("CameraServer::UserSetMaxFailedAttempts Failed - value exists: %s", request.isMember("value") ? "true" : "false");
		return -EINVAL;
	}
	int value = request["value"].asInt();
	int ret = User::SetMaxFailedAttempts(value);
	handler->Cfg->Dirty();
	return ret;
}

int CameraServer::UserGetAutoLogOff(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	return User::GetAutoLogOff();
}

int CameraServer::UserSetAutoLogOff(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	if (request.isMember("value") == false || request["value"].isInt() == false)
	{
		LogError("CameraServer::UserSetAutoLogOff Failed - value exists: %s", request.isMember("value") ? "true" : "false");
		return -EINVAL;
	}
	int value = request["value"].asInt();
	int ret = User::SetAutoLogOff(value);
	handler->Cfg->Dirty();
	return ret;
}

int CameraServer::UserInfo(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	if (request.isMember("Username") == false || request["Username"].isString() == false)
	{
		LogError("CameraServer::UserSetApproved Failed - Username exists: %s", request.isMember("Username") ? "true" : "false");
		return -EINVAL;
	}
	
	std::string Username = request["Username"].asString();
	UserItem info;
	
	int ret = User::UserInfo(Username, &info);
	if (ret < 0)
		return ret;

	info.ConfigSave(response);
	return 0;
}

int CameraServer::UserList(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	std::vector<std::string> lst = User::List();
	response["value"] = Json::arrayValue;
	for(auto str : lst)
	{
		response["value"].append(str);
	}
	return lst.size();
}

int CameraServer::WebServerGetPort(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	int port = handler->WServer->GetPort();
	response["value"] = port;
	return 0;
}

int CameraServer::WebServerSetPort(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	if (request.isMember("value") == false || request.isInt() == false)
	{
		LogError("CameraServer::WebServerSetPort Failed - Paramater '%s' is missing", request.isMember("port") ? "true" : "false");
		return -EINVAL;
	}
	
	int port = request["value"].asInt();
	
	if (port <= 0 || port > 65535)
	{
		LogError("CameraServer::WebServerSetPort Failed - %d Must be between 0 and 65535", port);
		return -EINVAL;
	}
	
	int ret = handler->WServer->SetPort(port);
	if (ret <= 0)
		return ret;
	handler->Cfg->Dirty();
	return ret;
}

int CameraServer::WebServerGetEnabled(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	response["enabled"] = handler->WServer->GetEnabled();
	return 0;
}

int CameraServer::WebServerSetEnabled(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	if (request.isMember("enabled") == false || request["enabled"].isBool() == false)
	{
		LogError("CameraServer::WebServerSetEnabled Failed - Paramater '%s' is missing", request.isMember("value") ? "true" : "false");
		return -EINVAL;
	}
	
	bool enabled = request["enabled"].asBool();
	
	handler->WServer->SetEnabled(enabled);
	handler->Cfg->Dirty();
	return 0;
}

int CameraServer::WebServerGetProperty(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	if (request.isMember("key") == false || request["key"].isString() == false)
	{
		LogError("CameraServer::WebServerGetProperty - Missing argument 'key': %s", request.isMember("key") ? "true" : "false");
		return -EINVAL;
	}
	
	std::string key = request["key"].asString();
	
	if (request.isMember("default") && request["default"].isString())
	{
		std::string def = request["default"].asString();
		response["value"] = handler->WServer->GetProperty(key, def);
	}
	else
	{
		response["value"] = handler->WServer->GetProperty(key);
	}

	return 0;
}

int CameraServer::WebServerSetProperty(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	if (request.isMember("key") == false || request["key"].isString() == false)
	{
		LogError("CameraServer::WebServerSetProperty - Missing argument 'key': %s", request.isMember("key") ? "true" : "false");
		return -EINVAL;
	}
	
	if (request.isMember("value") == false || request["value"].isString() == false)
	{
		LogError("CameraServer::WebServerSetProperty - Missing argument 'value': %s", request.isMember("value") ? "true" : "false");
		return -EINVAL;
	}

	std::string key = request["key"].asString();
	std::string value = request["value"].asString();
	
	handler->WServer->SetProperty(key, value);
	handler->Cfg->Dirty();
	return 0;
}


int CameraServer::WebServerRestart(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	handler->WServer->Restart();
	return 0;
}

int CameraServer::WebStreamStart(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	std::string str;
	WebStreamOptions options;

	if (request.isMember("options") == false)
	{
		LogError("CameraServer::WebServerRestart Failed - exists: %s", request.isMember("options") ? "true" : "false");
		return -EINVAL;
	}

//	options.Decode(request["options"]);

	return handler->WStream->StartVideoInput(&options);
}

int CameraServer::DebugGetEnabled(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	bool enabled = Debug::GetEnabled();
	response["enabled"] = enabled;
	return 0;
}

int CameraServer::DebugSetEnabled(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	if (request.isMember("enabled") == false || request["enabled"].isBool() == false)
	{
		LogError("CameraServer::WebServerSetEnabled Failed - Paramater '%s' is missing", request.isMember("value") ? "true" : "false");
		return -EINVAL;
	}
	bool enabled = request["enabled"].asBool();

	Debug::SetEnabled(enabled);
	handler->Cfg->Dirty();
	return 0;
}

int CameraServer::Version(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	LogDebug("CameraServer::Version");
	response["value"] = Version::ToString();
	return 0;
}

int CameraServer::Ping(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	LogDebug("CameraServer::Ping PING");
	return 0;
}

int CameraServer::Quit(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	LogDebug("CameraServer::Quit QUIT");
	handler->Quit();
	return 0;
}

int CameraServer::Log(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	if (request.isMember("Level") == false || request["Level"].isString() == false)
	{
		LogError("CameraServer::Log - Missing argument 'Level': %s", request.isMember("Level") ? "true" : "false");
		return -EINVAL;
	}
	
	if (request.isMember("Message") == false || request["Message"].isString() == false)
	{
		LogError("CameraServer::Log - Missing argument 'Message': %s", request.isMember("Message") ? "true" : "false");
		return -EINVAL;
	}
	
	std::string Level = request["Level"].asString();
	std::string Message = request["Message"].asString();
	
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

int CameraServer::StatsList(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	LogDebug("CameraServer::StatsList");
	std::list<std::string> Keys = PerfManager::GetKeyList();	
	response["list"] = Json::arrayValue;
	
	for(auto str : Keys)
	{
		response["list"].append(str);
	}

	return Keys.size();
}

int CameraServer::StatsInfo(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	LogDebug("CameraServer::StatsInfo");
	struct PerfDetails_t info;
	
	if (request.isMember("key") == false || request["key"].isString())
	{
		LogError("CameraServer::StatsInfo - Missing argument 'key': %s", request.isMember("key") ? "true" : "false");
		return -EINVAL;
	}
	
	std::string Key = request["key"].asString();
	
	if (PerfManager::GetInfo(Key, &info) < 0)
		return -ENOLINK;

	if (info.Count == 0)
	{
		response["AverageTime"] = 0;
	}
	else
	{
		response["AverageTime"] = (int) (Time::MicroSeconds(&info.TotalTime) / info.Count);
	}
	response["WordtTime"] = (int) Time::MicroSeconds(&info.WorstTime);
	response["BestTime"] = (int) Time::MicroSeconds(&info.BestTime);
	response["Count"] = info.Count;
	return 0;
}

int CameraServer::StatsDump(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	LogDebug("CameraServer::StatsDump");
	StatsDump();
}

int CameraServer::StatsReset(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	LogInfo("CameraServer::StatsReset");
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
		LogInfo("%25s Total Calls: %6d Average Time: %" PRIu64 " Micro Seconds",
				it->c_str(), info.Count,
				Time::MicroSeconds(&info.TotalTime) / info.Count);
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
		
int CameraServer::OnRequest(IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	TimerAbort abrt = TimerAbort(120);
	
	if (request.isMember("action") == false || request["action"].isString() == false)
	{
		LogError("CameraServer::OnRequest No 'action'");
		return -EINVAL;
	}
	
	std::string Command = request["action"].asString();
	const TType *tmp = CameraServerFunctions::Lookup(Command.c_str(), Command.length());
	if (tmp == NULL)
	{
		LogError("CameraServer::OnRequest Unknown Command: \"%s\"", Command.c_str());
		return -ENOSYS;
	}
	PerfCounter PC(Command.c_str());
	CameraTimers->Add(&abrt);
	int ret = tmp->func(m_handler, Connection, request, response);
	CameraTimers->Remove(&abrt);
	return ret;
}

int CameraServer::OnCommand(IServerConnection *Connection, Json::Value &request)
{
	LogDebug("CameraServer::OnCommand");
	return -ENOSYS;
}

void CameraServer::OnBadLine(IServerConnection *Connection, const std::string *line)
{
	LogError("CameraServer::OnBadLine(%s)", line->c_str());
}



