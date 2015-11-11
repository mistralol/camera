
#include <json/json.h>

#include <libclientserver.h>

#include <WebStreamType.h>
#include <WebStreamOptions.h>
#include <VideoInputSupported.h>
#include <VideoInputConfig.h>
#include <VideoOutputSupported.h>
#include <VideoOutputTourItem.h>
#include <VideoOutputTour.h>
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

void CameraClient::RTSPKickAll()
{
	PerfCounter PC("RTSPKickAll");
	Request request;
	Request response;

	request.SetCommand("RTSPKickAll");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

int CameraClient::RTSPGetPort()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("RTSPGetPort");
	Request request;
	Request response;

	request.SetCommand("RTSPGetPort");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	if (response.GetInt("value", &ret) == false)
		throw(CameraClientException(-EINVAL));

	return ret;
}

void CameraClient::RTSPSetPort(int value)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("RTSPSetPort");
	Request request;
	Request response;

	request.SetCommand("RTSPSetPort");
	request.SetArg("value", value);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

int CameraClient::RTSPGetClientCount()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("RTSPGetClientCount");
	Request request;
	Request response;

	request.SetCommand("RTSPGetClientCount");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	int value = 0;
	if (response.GetInt("value", &value) == false)
		throw(CameraClientException(-EINVAL));
	return value;
}

void CameraClient::RTSPSetMaxClients(int max)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("RTSPSetMaxClients");
	Request request;
	Request response;

	request.SetCommand("RTSPSetMaxClients");
	request.SetArg("value", max);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

int CameraClient::RTSPGetMaxClients()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("RTSPGetMaxClients");
	Request request;
	Request response;

	request.SetCommand("RTSPGetMaxClients");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	int max = 0;
	if (response.GetInt("value", &max) == false)
		return -EINVAL;

	return max;
}

void CameraClient::RTSPSetMaxBacklog(int max)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("RTSPSetMaxBacklog");
	Request request;
	Request response;

	request.SetCommand("RTSPSetMaxBacklog");
	request.SetArg("value", max);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

int CameraClient::RTSPGetMaxBacklog()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("RTSPGetMaxBacklog");
	Request request;
	Request response;

	request.SetCommand("RTSPGetMaxBacklog");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	int max = 0;
	if (response.GetInt("value", &max) == false)
		throw(CameraClientException(ret));

	return max;
}

std::string CameraClient::Version()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("VERSION");
	Request request;
	Request response;

	request.SetCommand("VERSION");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	std::string str = "Unknown";
	if (response.GetString("value", &str) == false)
		throw(CameraClientException(-EINVAL));
	return str;
}

int CameraClient::VideoInputCount()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("VideoInputCount");
	Request request;
	Request response;

	request.SetCommand("VideoInputCount");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	int value = 0;
	if (response.GetInt("value", &value) == false)
		return -EINVAL;
	return value;
}

void CameraClient::VideoInputSetEnabled(int input, bool enabled)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("VideoInputSetEnabled");
	Request request;
	Request response;

	request.SetCommand("VideoInputSetEnabled");
	request.SetArg("input", input);
	request.SetArg("enabled", enabled);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

int CameraClient::VideoInputGetEnabled(int input)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("VideoInputGetEnabled");
	Request request;
	Request response;

	request.SetCommand("VideoInputGetEnabled");
	request.SetArg("input", input);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	int value = 0;
	if (response.GetInt("enabled", &value) == false)
		throw(CameraClientException(-EINVAL));

	return value;
}

VideoInputConfig CameraClient::VideoInputGetConfig(int input)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("VideoInputGetConfig");
	Request request;
	Request response;
	
	request.SetCommand("VideoInputGetConfig");
	request.SetArg("input", input);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	std::string str = "";
	if (response.GetString("config", &str) == false)
		throw(CameraClientException(-EINVAL));
	VideoInputConfig cfg;
	if (cfg.Decode(str) == false)
		throw(CameraClientException(-EINVAL));
	return cfg;
}

void CameraClient::VideoInputSetConfig(int input, VideoInputConfig *cfg)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("VideoInputSetConfig");
	Request request;
	Request response;
	
	request.SetCommand("VideoInputSetConfig");
	request.SetArg("input", input);
	request.SetArg("config", cfg->Encode());
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

VideoInputSupported CameraClient::VideoInputGetSupported(int input)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("VideoInputGetSupported");
	Request request;
	Request response;
	
	request.SetCommand("VideoInputGetSupported");
	request.SetArg("input", input);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	std::string str = "";
	if (response.GetString("info", &str) == false)
		throw(CameraClientException(-EINVAL));
	VideoInputSupported info;
	if (info.Decode(str) == false)
		throw(CameraClientException(-EINVAL));
	return info;
}

int CameraClient::VideoOutputCount()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("VideoOutputCount");
	Request request;
	Request response;

	request.SetCommand("VideoOutputCount");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	int value = 0;
	if (response.GetInt("value", &value) == false)
		return -EINVAL;
	return value;
}

VideoOutputSupported CameraClient::VideoOutputGetSupported(int output)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("VideoOutputGetSupported");
	Request request;
	Request response;
	
	request.SetCommand("VideoOutputGetSupported");
	request.SetArg("output", output);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	std::string str = "";
	if (response.GetString("info", &str) == false)
		throw(CameraClientException(-EINVAL));
	VideoOutputSupported info;
	if (info.Decode(str) == false)
		throw(CameraClientException(-EINVAL));
	return info;
}
		
std::vector<std::string> CameraClient::VideoOutputTourList()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("VideoOutputTourList");
	Request request;
	Request response;

	request.SetCommand("VideoOutputTourList");

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	std::vector<std::string> lst;
	if (response.GetVectorString("info", &lst) == false)
		throw(CameraClientException(-EINVAL));
	return lst;
}

void CameraClient::VideoOutputTourAdd(VideoOutputTour *tour)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("VideoOutputTourAdd");
	Request request;
	Request response;
	
	request.SetCommand("VideoOutputTourAdd");
	request.SetArg("info", tour->Encode());
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

void CameraClient::VideoOutputTourUpdate(VideoOutputTour *tour)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("VideoOutputTourUpdate");
	Request request;
	Request response;

	request.SetCommand("VideoOutputTourUpdate");
	request.SetArg("info", tour->Encode());
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

VideoOutputTour CameraClient::VideoOutputTourGet(const std::string &name)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("VideoOutputTourGet");
	Request request;
	Request response;
	
	request.SetCommand("VideoOutputTourGet");
	request.SetArg("name", name);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	std::string str = "";
	if (response.GetString("info", &str) == false)
		throw(CameraClientException(-EINVAL));
	VideoOutputTour cfg;
	if (cfg.Decode(str) == false)
		throw(CameraClientException(-EINVAL));
	return cfg;
}

bool CameraClient::VideoOutputTourExists(const std::string &name)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("VideoOutputTourExists");
	Request request;
	Request response;
	
	request.SetCommand("VideoOutputTourExists");
	request.SetArg("name", name);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	if (ret)
		return true;
	return false;
}

void CameraClient::VideoOutputTourRemove(const std::string &name)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("VideoOutputTourRemove");
	Request request;
	Request response;
	
	request.SetCommand("VideoOutputTourRemove");
	request.SetArg("name", name);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

int CameraClient::GPIOOutputCount()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("GPIOOutputCount");
	Request request;
	Request response;
	
	request.SetCommand("GPIOOutputCount");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	int value = 0;
	if (response.GetInt("value", &value) == false)
		throw(CameraClientException(-EINVAL));
	return value;
}

void CameraClient::GPIOOutputSetState(int output, bool enabled)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("GPIOOutputSetState");
	Request request;
	Request response;
	
	request.SetCommand("GPIOOutputSetState");
	request.SetArg("output", output);
	request.SetArg("value", enabled);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

void CameraClient::GPIOOutputSetState(int output, bool enabled, const struct timespec *ts)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("GPIOOutputSetState");
	Request request;
	Request response;
	
	request.SetCommand("GPIOOutputSetState");
	request.SetArg("output", output);
	request.SetArg("value", enabled);
	request.SetArg("timeout", ts);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

bool CameraClient::GPIOOutputGetState(int output)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("GPIOOutputGetState");
	Request request;
	Request response;
	
	request.SetCommand("GPIOOutputGetState");
	request.SetArg("output", output);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	if (ret)
		return true;
	return false;
}

void CameraClient::UserCreate(const std::string Username, const std::string Password, const std::string EMail)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("UserCreate");
	Request request;
	Request response;

	request.SetCommand("UserCreate");
	request.SetArg("Username", Username);
	request.SetArg("Password", Password);
	request.SetArg("EMail", EMail);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));

}

bool CameraClient::UserAuth(const std::string Username, const std::string Password)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("UserAuth");
	Request request;
	Request response;

	request.SetCommand("UserAuth");
	request.SetArg("Username", Username);
	request.SetArg("Password", Password);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (ret)
		return true;
	return false;
}

void CameraClient::UserDelete(const std::string Username)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("UserDelete");
	Request request;
	Request response;

	request.SetCommand("UserDelete");
	request.SetArg("Username", Username);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

bool CameraClient::UserExists(const std::string Username)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("UserExists");
	Request request;
	Request response;

	request.SetCommand("UserExists");
	request.SetArg("Username", Username);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	if (ret)
		return true;
	return false;
}

void CameraClient::UserTouch(const std::string Username)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("UserTouch");
	Request request;
	Request response;

	request.SetCommand("UserTouch");
	request.SetArg("Username", Username);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

bool CameraClient::UserIsLockedOut(const std::string Username)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("UserIsLockedOut");
	Request request;
	Request response;

	request.SetCommand("UserIsLockedOut");
	request.SetArg("Username", Username);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	if (ret)
		return true;
	return false;
}

bool CameraClient::UserIsApproved(const std::string Username)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("UserIsApproved");
	Request request;
	Request response;

	request.SetCommand("UserIsApproved");
	request.SetArg("Username", Username);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	if (ret)
		return true;
	return false;
}

bool CameraClient::UserIsOnline(const std::string Username)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("UserIsOnline");
	Request request;
	Request response;

	request.SetCommand("UserIsOnline");
	request.SetArg("Username", Username);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	if (ret)
		return true;
	return false;
}

void CameraClient::UserSetLockedOut(const std::string Username, bool value)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("UserSetLockedOut");
	Request request;
	Request response;

	request.SetCommand("UserSetLockedOut");
	request.SetArg("Username", Username);
	request.SetArg("value", value);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

void CameraClient::UserSetApproved(const std::string Username, bool value)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("UserSetApproved");
	Request request;
	Request response;

	request.SetCommand("UserSetApproved");
	request.SetArg("Username", Username);
	request.SetArg("value", value);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

void CameraClient::UserSetPassword(const std::string Username, const std::string Password)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("UserSetPassword");
	Request request;
	Request response;

	request.SetCommand("UserSetPassword");
	request.SetArg("Username", Username);
	request.SetArg("Password", Password);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

int CameraClient::UserGetLockoutDuration()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("UserGetLockoutDuration");
	Request request;
	Request response;

	request.SetCommand("UserGetLockoutDuration");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	return ret;
}

void CameraClient::UserSetLockoutDuration(int value)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("UserSetLockoutDuration");
	Request request;
	Request response;

	request.SetCommand("UserSetLockoutDuration");
	request.SetArg("value", value);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

int CameraClient::UserGetMaxFailedAttempts()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("UserGetMaxFailedAttempts");
	Request request;
	Request response;

	request.SetCommand("UserGetMaxFailedAttempts");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

void CameraClient::UserSetMaxFailedAttempts(int value)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("UserSetMaxFailedAttempts");
	Request request;
	Request response;

	request.SetCommand("UserSetMaxFailedAttempts");
	request.SetArg("value", value);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

int CameraClient::UserGetAutoLogOff()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("UserGetAutoLogOff");
	Request request;
	Request response;

	request.SetCommand("UserGetAutoLogOff");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	return ret;
}

void CameraClient::UserSetAutoLogOff(int value)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("UserSetAutoLogOff");
	Request request;
	Request response;

	request.SetCommand("UserSetAutoLogOff");
	request.SetArg("value", value);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

void CameraClient::UserInfo(const std::string Username, struct UserItem *info)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("UserInfo");
	Request request;
	Request response;

	request.SetCommand("UserInfo");
	request.SetArg("Username", Username);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	std::string str;
	if (response.GetString("info", &str) == false)
		throw(CameraClientException(-EINVAL));
	if (info->Decode(str) == false)
		throw(CameraClientException(-EINVAL));
}

std::vector<std::string> CameraClient::UserList()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("UserList");
	Request request;
	Request response;

	request.SetCommand("UserList");

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	std::vector<std::string> lst;
	if (response.GetVectorString("value", &lst) == false)
		throw(CameraClientException(-EINVAL));
	return lst;
}

std::string CameraClient::UserGetUserFromEMail(const std::string EMail)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("UserGetUserFromEMail");
	Request request;
	Request response;

	request.SetCommand("UserGetUserFromEMail");
	request.SetArg("EMail", EMail);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	std::string str = "";
	if (response.GetString("Username", &str) == false)
		throw(CameraClientException(-EINVAL));
	return str;
}

std::string CameraClient::UserGetUserFromKey(const std::string Key)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("UserGetUserFromKey");
	Request request;
	Request response;

	request.SetCommand("UserGetUserFromKey");
	request.SetArg("Key", Key);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	std::string str = "";
	if (response.GetString("Username", &str) == false)
		throw(CameraClientException(-EINVAL));
	return str;
}

void CameraClient::GroupCreate(const std::string Group)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("GroupCreate");
	Request request;
	Request response;

	request.SetCommand("GroupCreate");
	request.SetArg("Group", Group);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

void CameraClient::GroupDelete(const std::string Group)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("GroupDelete");
	Request request;
	Request response;

	request.SetCommand("GroupDelete");
	request.SetArg("Group", Group);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

bool CameraClient::GroupExists(const std::string Group)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("GroupExists");
	Request request;
	Request response;

	request.SetCommand("GroupExists");
	request.SetArg("Group", Group);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	if (ret)
		return true;
	return false;
}

bool CameraClient::GroupIsUserInGroup(const std::string Group, const std::string User)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("GroupIsUserInGroup");
	Request request;
	Request response;

	request.SetCommand("GroupIsUserInGroup");
	request.SetArg("Group", Group);
	request.SetArg("Username", User);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	if (ret)
		return true;
	return false;
}

void CameraClient::GroupUserAdd(const std::string Group, const std::string User)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("GroupUserAdd");
	Request request;
	Request response;

	request.SetCommand("GroupUserAdd");
	request.SetArg("Group", Group);
	request.SetArg("Username", User);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

void CameraClient::GroupUserRemove(const std::string Group, const std::string User)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("GroupUserRemove");
	Request request;
	Request response;

	request.SetCommand("GroupUserRemove");
	request.SetArg("Group", Group);
	request.SetArg("Username", User);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

std::vector<std::string> CameraClient::GroupList()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("GroupList");
	Request request;
	Request response;

	request.SetCommand("GroupList");

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	std::vector<std::string> lst;
	if (response.GetVectorString("value", &lst) == false)
		throw(CameraClientException(-EINVAL));
	return lst;
}

std::vector<std::string> CameraClient::GroupListUsers(const std::string Group)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("GroupListUsers");
	Request request;
	Request response;

	request.SetCommand("GroupListUsers");
	request.SetArg("Group", Group);

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	std::vector<std::string> lst;
	if (response.GetVectorString("value", &lst) == false)
		throw(CameraClientException(-EINVAL));
	return lst;
}

int CameraClient::WebServerGetPort()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("WebServerGetPort");
	Request request;
	Request response;
	
	request.SetCommand("WebServerGetPort");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	int port = 0;
	if (response.GetInt("port", &port) == false)
		return -EINVAL;
	return port;
}

void CameraClient::WebServerSetPort(int port)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("WebServerSetPort");
	Request request;
	Request response;
	
	request.SetCommand("WebServerSetPort");
	request.SetArg("port", port);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

bool CameraClient::WebServerGetEnabled()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("WebServerGetEnabled");
	Request request;
	Request response;
	
	request.SetCommand("WebServerGetEnabled");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	int enabled = 0;
	if (response.GetInt("enabled", &enabled) == false)
		return -EINVAL;
	if (enabled)
		return true;
	return false;
}

void CameraClient::WebServerSetEnabled(bool enabled)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("WebServerSetEnabled");
	Request request;
	Request response;
	
	request.SetCommand("WebServerSetEnabled");
	request.SetArg("enabled", enabled);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

std::string CameraClient::WebServerGetProperty(const std::string key, const std::string def)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("WebServerGetProperty");
	Request request;
	Request response;

	request.SetCommand("WebServerGetProperty");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
	        throw(CameraClientException(ret));
	std::string value = "";
	if (response.GetString("value", &value) == false)
		throw(CameraClientException(-EINVAL));
	return value;
}

void CameraClient::WebServerSetProperty(const std::string key, const std::string value)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("WebServerSetProperty");
	Request request;
	Request response;
	
	request.SetCommand("WebServerSetProperty");
	request.SetArg("key", key);
	request.SetArg("value", value);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

void CameraClient::WebServerRestart()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("WebServerRestart");
	Request request;
	Request response;
	
	request.SetCommand("WebServerRestart");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

int CameraClient::WebStreamStart(WebStreamOptions *options)
{
	std::string str;
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("WebStreamStart");
	Request request;
	Request response;
	request.SetCommand("WebStreamStart");
	if (options->Encode(str) == false)
		throw(CameraClientException(-EINVAL));
	request.SetArg("options", str);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	return ret; // return port number
}

void CameraClient::DebugSetEnabled(bool enabled)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("DebugSetEnabled");
	Request request;
	Request response;
	
	request.SetCommand("DebugSetEnabled");
	request.SetArg("enabled", enabled);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

bool CameraClient::DebugGetEnabled()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("DebugGetEnabled");
	Request request;
	Request response;
	
	request.SetCommand("DebugGetEnabled");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	int enabled = 0;
	if (response.GetInt("enabled", &enabled) == false)
		return -EINVAL;
	
	if (enabled)
		return true;
	return false;	

}

void CameraClient::SystemReboot()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("SystemReboot");
	Request request;
	Request response;

	request.SetCommand("SystemReboot");

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

void CameraClient::SystemShutdown()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("SystemShutdown");
	Request request;
	Request response;

	request.SetCommand("SystemShutdown");

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

void CameraClient::Ping()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("PING");
	Request request;
	Request response;

	request.SetCommand("PING");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

void CameraClient::Quit()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("QUIT");
	Request request;
	Request response;

	request.SetCommand("QUIT");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

void CameraClient::Log(const std::string Level, const std::string Message)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("LOG");
	Request request;
	Request response;
	
	request.SetCommand("LOG");
	request.SetArg("Level", Level);
	request.SetArg("Message", Message);
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

void CameraClient::LogDebug(const std::string Message)
{
	return Log("DEBUG", Message);
}

void CameraClient::LogInfo(const std::string Message)
{
	return Log("INFO", Message);
}

void CameraClient::LogNotice(const std::string Message)
{
	return Log("NOTICE", Message);
}

void CameraClient::LogWarning(const std::string Message)
{
	return Log("WARNING", Message);
}

void CameraClient::LogError(const std::string Message)
{
	return Log("ERROR", Message);
}

void CameraClient::LogCritical(const std::string Message)
{
	return Log("CRITICAL", Message);
}

void CameraClient::LogAlert(const std::string Message)
{
	return Log("ALERT", Message);
}

void CameraClient::LogEmergency(const std::string Message)
{
	return Log("EMERGENCY", Message);
}

std::vector<std::string> CameraClient::StatsList()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("StatsList");
	Request request;
	Request response;

	request.SetCommand("StatsList");

	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	std::vector<std::string> lst;
	if (response.GetVectorString("list", &lst) == false)
		throw(CameraClientException(-EINVAL));
	return lst;
}

void CameraClient::StatsInfo(const std::string key, struct StatsInfo *info)
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("StatsInfo");
	Request request;
	Request response;
	
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
	if (response.GetInt("TotalTime", &info->TotalTime) == false)
		throw(CameraClientException("Unable to parse TotalTime"));
	if (response.GetInt("WorstTime", &info->WorstTime) == false)
		throw(CameraClientException("Unable to parse WorstTime"));
	if (response.GetInt("BestTime", &info->BestTime) == false)
		throw(CameraClientException("Unable to parse BestTime"));
	if (response.GetInt("Count", &info->Count) == false)
		throw(CameraClientException("Unable to parse Count"));

}

void CameraClient::StatsDump()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("StatsDump");
	Request request;
	Request response;
	
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}

void CameraClient::StatsReset()
{
	if (m_Client == NULL)
		throw(CameraClientException(ENOTCONN));
	PerfCounter PC("StatsReset");
	Request request;
	Request response;
	
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		throw(CameraClientException(ret));
}




