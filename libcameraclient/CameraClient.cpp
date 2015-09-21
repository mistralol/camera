
#include <json/json.h>

#include <libclientserver.h>

#include <WebStreamType.h>
#include <WebStreamOptions.h>
#include <VideoInputSupported.h>
#include <VideoInputConfig.h>
#include <UserItem.h>

#include <CameraClient.h>

CameraClient::CameraClient()
{

}

CameraClient::~CameraClient()
{
	if (m_Client)
	{
		Disconnect();
	}
}

bool CameraClient::Connect(const std::string ConnectionString)
{
	struct timespec soft = {120, 0};
	struct timespec hard = {300, 0};
	struct timespec reconnect = {5, 0};
	m_Client = Client::Create(ConnectionString);

	m_Client->SetSoftTimeout(&soft);
	m_Client->SetHardTimeout(&hard);
	m_Client->SetReConnectDelay(&reconnect);

	m_Client->Connect();
	m_Client->WaitForConnect();
	return true;
}

void CameraClient::Disconnect()
{
	if (m_Client == NULL)
		return;
	m_Client->Disconnect();
	delete m_Client;
	m_Client = NULL;
}

int CameraClient::RTSPKickAll()
{
	PerfCounter PC("RTSPKickAll");
	Request request;
	Request response;

	request.SetCommand("RTSPKickAll");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;

	return ret;
}

int CameraClient::RTSPGetPort(int *value)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("RTSPGetPort");
	Request request;
	Request response;

	request.SetCommand("RTSPGetPort");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (response.GetInt("value", value) == false)
		return -EINVAL;

	return ret;
}

int CameraClient::RTSPSetPort(int value)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("RTSPSetPort");
	Request request;
	Request response;

	request.SetCommand("RTSPSetPort");
	request.SetArg("value", value);
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::RTSPGetClientCount(int *value)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("RTSPGetClientCount");
	Request request;
	Request response;

	request.SetCommand("RTSPGetClientCount");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (response.GetInt("value", value) == false)
		return -EINVAL;
	
	return ret;
}

int CameraClient::RTSPSetMaxClients(int max)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("RTSPSetMaxClients");
	Request request;
	Request response;

	request.SetCommand("RTSPSetMaxClients");
	request.SetArg("value", max);
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::RTSPGetMaxClients(int *max)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("RTSPGetMaxClients");
	Request request;
	Request response;

	request.SetCommand("RTSPGetMaxClients");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (response.GetInt("value", max) == false)
		return -EINVAL;

	return ret;
}

int CameraClient::RTSPSetMaxBacklog(int max)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("RTSPSetMaxBacklog");
	Request request;
	Request response;

	request.SetCommand("RTSPSetMaxBacklog");
	request.SetArg("value", max);
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::RTSPGetMaxBacklog(int *max)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("RTSPGetMaxBacklog");
	Request request;
	Request response;

	request.SetCommand("RTSPGetMaxBacklog");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (response.GetInt("value", max) == false)
		return -EINVAL;

	return ret;
}

int CameraClient::Version(std::string *str)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("VERSION");
	Request request;
	Request response;

	request.SetCommand("VERSION");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (response.GetString("value", str) == false)
		return -EINVAL;
	return ret;
}

int CameraClient::VideoInputCount(int *value)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("VideoInputCount");
	Request request;
	Request response;

	request.SetCommand("VideoInputCount");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (response.GetInt("value", value) == false)
		return -EINVAL;
	return 0;
}

int CameraClient::VideoInputSetEnabled(int input, bool enabled)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("VideoInputSetEnabled");
	Request request;
	Request response;

	request.SetCommand("VideoInputSetEnabled");
	request.SetArg("input", input);
	request.SetArg("enabled", enabled);
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::VideoInputGetEnabled(int input, int *value)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("VideoInputGetEnabled");
	Request request;
	Request response;

	request.SetCommand("VideoInputGetEnabled");
	request.SetArg("input", input);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (response.GetInt("enabled", value) == false)
		return -EINVAL;

	return ret;
}

int CameraClient::VideoInputGetConfig(int input, VideoInputConfig *cfg)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("VideoInputGetConfig");
	Request request;
	Request response;
	
	request.SetCommand("VideoInputGetConfig");
	request.SetArg("input", input);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	std::string str = "";
	if (response.GetString("config", &str) == false)
		return -EINVAL;
	if (cfg->Decode(str) == false)
		return -EINVAL;
	return ret;
}

int CameraClient::VideoInputSetConfig(int input, VideoInputConfig *cfg)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("VideoInputGetConfig");
	Request request;
	Request response;
	
	request.SetCommand("VideoInputGetConfig");
	request.SetArg("input", input);
	request.SetArg("config", cfg->Encode());
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::VideoInputGetSupport(int input, VideoInputSupported *info)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("VideoInputGetSupport");
	Request request;
	Request response;
	
	request.SetCommand("VideoInputGetSupport");
	request.SetArg("input", input);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	std::string str = "";
	if (response.GetString("info", &str) == false)
		return -EINVAL;
	if (info->Decode(str) == false)
		return -EINVAL;
	return ret;
}

int CameraClient::UserCreate(const std::string Username, const std::string Password, const std::string EMail)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("UserCreate");
	Request request;
	Request response;

	request.SetCommand("UserCreate");
	request.SetArg("Username", Username);
	request.SetArg("Password", Password);
	request.SetArg("EMail", EMail);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	return ret;
}

int CameraClient::UserAuth(const std::string Username, const std::string Password)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("UserAuth");
	Request request;
	Request response;

	request.SetCommand("UserAuth");
	request.SetArg("Username", Username);
	request.SetArg("Password", Password);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	return ret;
}

int CameraClient::UserDelete(const std::string Username)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("UserDelete");
	Request request;
	Request response;

	request.SetCommand("UserDelete");
	request.SetArg("Username", Username);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	return ret;
}

int CameraClient::UserExists(const std::string Username)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("UserExists");
	Request request;
	Request response;

	request.SetCommand("UserExists");
	request.SetArg("Username", Username);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	return ret;
}

int CameraClient::UserTouch(const std::string Username)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("UserTouch");
	Request request;
	Request response;

	request.SetCommand("UserTouch");
	request.SetArg("Username", Username);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	return ret;
}

int CameraClient::UserIsLockedOut(const std::string Username)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("UserIsLockedOut");
	Request request;
	Request response;

	request.SetCommand("UserIsLockedOut");
	request.SetArg("Username", Username);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	return ret;
}

int CameraClient::UserIsApproved(const std::string Username)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("UserIsApproved");
	Request request;
	Request response;

	request.SetCommand("UserIsApproved");
	request.SetArg("Username", Username);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	return ret;
}

int CameraClient::UserIsOnline(const std::string Username)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("UserIsOnline");
	Request request;
	Request response;

	request.SetCommand("UserIsOnline");
	request.SetArg("Username", Username);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	return ret;
}

int CameraClient::UserSetLockedOut(const std::string Username, bool value)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("UserSetLockedOut");
	Request request;
	Request response;

	request.SetCommand("UserSetLockedOut");
	request.SetArg("Username", Username);
	request.SetArg("value", value);
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::UserSetApproved(const std::string Username, bool value)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("UserSetApproved");
	Request request;
	Request response;

	request.SetCommand("UserSetApproved");
	request.SetArg("Username", Username);
	request.SetArg("value", value);
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::UserSetPassword(const std::string Username, const std::string Password)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("UserSetPassword");
	Request request;
	Request response;

	request.SetCommand("UserSetPassword");
	request.SetArg("Username", Username);
	request.SetArg("Password", Password);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	return ret;
}

int CameraClient::UserGetLockoutDuration()
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("UserGetLockoutDuration");
	Request request;
	Request response;

	request.SetCommand("UserGetLockoutDuration");
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::UserSetLockoutDuration(int value)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("UserSetLockoutDuration");
	Request request;
	Request response;

	request.SetCommand("UserSetLockoutDuration");
	request.SetArg("value", value);
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::UserGetMaxFailedAttempts()
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("UserGetMaxFailedAttempts");
	Request request;
	Request response;

	request.SetCommand("UserGetMaxFailedAttempts");
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::UserSetMaxFailedAttempts(int value)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("UserSetMaxFailedAttempts");
	Request request;
	Request response;

	request.SetCommand("UserSetMaxFailedAttempts");
	request.SetArg("value", value);
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::UserGetAutoLogOff()
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("UserGetAutoLogOff");
	Request request;
	Request response;

	request.SetCommand("UserGetAutoLogOff");
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::UserSetAutoLogOff(int value)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("UserSetAutoLogOff");
	Request request;
	Request response;

	request.SetCommand("UserSetAutoLogOff");
	request.SetArg("value", value);
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::UserInfo(const std::string Username, struct UserItem *info)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("UserInfo");
	Request request;
	Request response;

	request.SetCommand("UserInfo");
	request.SetArg("Username", Username);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	std::string str;
	if (response.GetString("info", &str) == false)
		return -EINVAL;
	if (info->Decode(str) == false)
		return -EINVAL;
	return ret;
}

int CameraClient::UserList(std::vector<std::string> &lst)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("UserList");
	Request request;
	Request response;

	request.SetCommand("UserList");

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (response.GetVectorString("value", &lst) == false)
		return -6;
	return lst.size();
}

int CameraClient::UserGetUserFromEMail(const std::string EMail, std::string &User)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("UserGetUserFromEMail");
	Request request;
	Request response;

	request.SetCommand("UserGetUserFromEMail");
	request.SetArg("EMail", EMail);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (response.GetString("Username", &User) == false)
		return -6;
	return ret;
}

int CameraClient::UserGetUserFromEMail(const std::string EMail, std::vector<std::string> &User)
{
	std::string username = "";
	int ret = UserGetUserFromEMail(EMail, username);
	User.clear();
	if (ret < 0)
		return ret;
	User.push_back(username);
	return ret;
}

int CameraClient::UserGetUserFromKey(const std::string Key, std::string &User)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("UserGetUserFromKey");
	Request request;
	Request response;

	request.SetCommand("UserGetUserFromKey");
	request.SetArg("Key", Key);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (response.GetString("Username", &User) == false)
		return -6;
	return ret;
}

int CameraClient::UserGetUserFromKey(const std::string Key, std::vector<std::string> &User)
{
	std::string username = "";
	int ret = UserGetUserFromKey(Key, username);
	User.clear();
	if (ret < 0)
		return ret;
	User.push_back(username);
	return ret;
}

int CameraClient::GroupCreate(const std::string Group)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("GroupCreate");
	Request request;
	Request response;

	request.SetCommand("GroupCreate");
	request.SetArg("Group", Group);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	return ret;
}

int CameraClient::GroupDelete(const std::string Group)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("GroupDelete");
	Request request;
	Request response;

	request.SetCommand("GroupDelete");
	request.SetArg("Group", Group);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	return ret;
}

int CameraClient::GroupExists(const std::string Group)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("GroupExists");
	Request request;
	Request response;

	request.SetCommand("GroupExists");
	request.SetArg("Group", Group);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	return ret;
}

int CameraClient::GroupIsUserInGroup(const std::string Group, const std::string User, int *result)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("GroupIsUserInGroup");
	Request request;
	Request response;

	request.SetCommand("GroupIsUserInGroup");
	request.SetArg("Group", Group);
	request.SetArg("Username", User);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	return ret;
}

int CameraClient::GroupUserAdd(const std::string Group, const std::string User)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("GroupUserAdd");
	Request request;
	Request response;

	request.SetCommand("GroupUserAdd");
	request.SetArg("Group", Group);
	request.SetArg("Username", User);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	return ret;
}

int CameraClient::GroupUserRemove(const std::string Group, const std::string User)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("GroupUserRemove");
	Request request;
	Request response;

	request.SetCommand("GroupUserRemove");
	request.SetArg("Group", Group);
	request.SetArg("Username", User);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	return ret;
}

int CameraClient::GroupList(std::vector<std::string> &lst)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("GroupList");
	Request request;
	Request response;

	request.SetCommand("GroupList");

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (response.GetVectorString("value", &lst) == false)
		return -1;
	return lst.size();
}

int CameraClient::GroupListUsers(const std::string Group, std::vector<std::string> &lst)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("GroupListUsers");
	Request request;
	Request response;

	request.SetCommand("GroupListUsers");
	request.SetArg("Group", Group);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (response.GetVectorString("value", &lst) == false)
		return -1;
	return lst.size();
}

int CameraClient::WebServerGetPort(int *port)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("WebServerGetPort");
	Request request;
	Request response;
	
	request.SetCommand("WebServerGetPort");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (response.GetInt("port", port) == false)
		return -EINVAL;

	return ret;
}

int CameraClient::WebServerSetPort(int port)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("WebServerSetPort");
	Request request;
	Request response;
	
	request.SetCommand("WebServerSetPort");
	request.SetArg("port", port);
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::WebServerGetEnabled(int *enabled)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("WebServerGetEnabled");
	Request request;
	Request response;
	
	request.SetCommand("WebServerGetEnabled");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (response.GetInt("enabled", enabled) == false)
		return -EINVAL;
		
	return 0;
}

int CameraClient::WebServerSetEnabled(bool enabled)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("WebServerSetEnabled");
	Request request;
	Request response;
	
	request.SetCommand("WebServerSetEnabled");
	request.SetArg("enabled", enabled);
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::WebServerGetProperty(const std::string key, const std::string def, std::string &value)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("WebServerGetProperty");
	Request request;
	Request response;
	
	request.SetCommand("WebServerGetProperty");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (response.GetString("value", &value) == false)
		return -EINVAL;
	return 0;
}

int CameraClient::WebServerGetProperty(const std::string key, const std::string def, std::vector<std::string> &value)
{
	std::string prop = "";
	int ret = WebServerGetProperty(key, def, prop);
	value.clear();
	if (ret < 0)
		return ret;
	value.push_back(prop);
	return ret;
}

int CameraClient::WebServerSetProperty(const std::string key, const std::string value)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("WebServerSetProperty");
	Request request;
	Request response;
	
	request.SetCommand("WebServerSetProperty");
	request.SetArg("key", key);
	request.SetArg("value", value);
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::WebServerRestart()
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("WebServerRestart");
	Request request;
	Request response;
	
	request.SetCommand("WebServerRestart");
	return m_Client->SendRequest(&request, &response);	
}

int CameraClient::WebStreamStart(WebStreamOptions *options)
{
	std::string str;
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("WebStreamStart");
	Request request;
	Request response;
	request.SetCommand("WebStreamStart");
	if (options->Encode(str) == false)
		return -EINVAL;
	request.SetArg("options", str);
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::DebugSetEnabled(bool enabled)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("DebugSetEnabled");
	Request request;
	Request response;
	
	request.SetCommand("DebugSetEnabled");
	request.SetArg("enabled", enabled);
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::DebugGetEnabled(int *enabled)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("DebugGetEnabled");
	Request request;
	Request response;
	
	request.SetCommand("DebugGetEnabled");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (response.GetInt("enabled", enabled) == false)
		return -EINVAL;
		
	return 0;
}

int CameraClient::SystemReboot()
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("SystemReboot");
	Request request;
	Request response;

	request.SetCommand("SystemReboot");

	return m_Client->SendRequest(&request, &response);
}

int CameraClient::SystemShutdown()
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("SystemShutdown");
	Request request;
	Request response;

	request.SetCommand("SystemShutdown");

	return m_Client->SendRequest(&request, &response);
}

std::string CameraClient::Version()
{
	std::string str = "";
	if (Version(&str) < 0)
		return "";
	return str;
}

int CameraClient::Ping()
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("PING");
	Request request;
	Request response;

	request.SetCommand("PING");
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::Quit()
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("QUIT");
	Request request;
	Request response;

	request.SetCommand("QUIT");
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::Log(const std::string Level, const std::string Message)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("LOG");
	Request request;
	Request response;
	
	request.SetCommand("LOG");
	request.SetArg("Level", Level);
	request.SetArg("Message", Message);
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::LogDebug(const std::string Message)
{
	return Log("DEBUG", Message);
}

int CameraClient::LogInfo(const std::string Message)
{
	return Log("INFO", Message);
}

int CameraClient::LogNotice(const std::string Message)
{
	return Log("NOTICE", Message);
}

int CameraClient::LogWarning(const std::string Message)
{
	return Log("WARNING", Message);
}

int CameraClient::LogError(const std::string Message)
{
	return Log("ERROR", Message);
}

int CameraClient::LogCritical(const std::string Message)
{
	return Log("CRITICAL", Message);
}

int CameraClient::LogAlert(const std::string Message)
{
	return Log("ALERT", Message);
}

int CameraClient::LogEmergency(const std::string Message)
{
	return Log("EMERGENCY", Message);
}

int CameraClient::StatsList(std::vector<std::string> &lst)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("StatsList");
	Request request;
	Request response;

	request.SetCommand("StatsList");

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (response.GetVectorString("list", &lst) == false)
		return -6;
	return lst.size();
}

int CameraClient::StatsInfo(const std::string key, struct StatsInfo *info)
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("StatsInfo");
	Request request;
	Request response;
	
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (response.GetInt("TotalTime", &info->TotalTime) == false)
		return -6;
	if (response.GetInt("WorstTime", &info->WorstTime) == false)
		return -7;
	if (response.GetInt("BestTime", &info->BestTime) == false)
		return -8;
	if (response.GetInt("Count", &info->Count) == false)
		return -9;
	return ret;
}

int CameraClient::StatsDump()
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("StatsDump");
	Request request;
	Request response;
	
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::StatsReset()
{
	if (m_Client == NULL)
		return -ENOTCONN;
	PerfCounter PC("StatsReset");
	Request request;
	Request response;
	
	return m_Client->SendRequest(&request, &response);
}

