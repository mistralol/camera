
class CameraClient
{
	public:
		CameraClient();
		~CameraClient();

		bool Connect(const std::string ConnectionString);
		void Disconnect();

		int RTSPKickAll();

		int RTSPGetPort(int *value);
		int RTSPSetPort(int value);

		int RTSPGetClientCount(int *value);

		int RTSPSetMaxClients(int max);
		int RTSPGetMaxClients(int *value);

		int RTSPSetMaxBacklog(int max);
		int RTSPGetMaxBacklog(int *value);

		int VideoInputCount(int *value);
		int VideoInputSetEnabled(int input, bool enabled);
		int VideoInputGetEnabled(int input, int *enabled);
		int VideoInputGetConfig(int input, VideoInputConfig *cfg);
		int VideoInputSetConfig(int input, VideoInputConfig *cfg);
		int VideoInputGetSupport(int input, VideoInputSupported *info);
		
		int UserCreate(const std::string Username, const std::string Password, const std::string EMail);
		int UserAuth(const std::string Username, const std::string Password);
		int UserDelete(const std::string Username);
		int UserExists(const std::string Username);
		int UserTouch(const std::string Username);
		int UserIsLockedOut(const std::string Username);
		int UserIsApproved(const std::string Username);
		int UserIsOnline(const std::string Username);
		int UserSetLockedOut(const std::string Username, bool value);
		int UserSetApproved(const std::string Username, bool value);
		int UserSetPassword(const std::string Username, const std::string Password);
		int UserGetLockoutDuration();
		int UserSetLockoutDuration(int value);
		int UserGetMaxFailedAttempts();
		int UserSetMaxFailedAttempts(int value);
		int UserGetAutoLogOff();
		int UserSetAutoLogOff(int value);
		int UserInfo(const std::string Username, struct UserItem *);
		int UserList(std::vector<std::string> &lst);
		int UserGetUserFromKey(const std::string Key, std::string &User);
		int UserGetUserFromKey(const std::string Key, std::vector<std::string> &User);
		

		int GroupCreate(const std::string Group);
		int GroupDelete(const std::string Group);
		int GroupExists(const std::string Group);
		int GroupIsUserInGroup(const std::string Group, const std::string User, int *result);
		int GroupUserAdd(const std::string Group, const std::string User);
		int GroupUserRemove(const std::string Group, const std::string User);
		int GroupList(std::vector<std::string> &lst);
		int GroupListUsers(const std::string Group, std::vector<std::string> &lst);

		int WebServerGetPort(int *port);
		int WebServerSetPort(int port);
		int WebServerGetEnabled(int *enabled);
		int WebServerSetEnabled(bool enabled);
		int WebServerGetProperty(const std::string key, const std::string def, std::string &value);
		int WebServerGetProperty(const std::string key, const std::string def, std::vector<std::string> &value);
		int WebServerSetProperty(const std::string key, const std::string value);
		int WebServerRestart();
		
		int WebStreamStart(WebStreamOptions *);
		
		int DebugSetEnabled(bool enabled);
		int DebugGetEnabled(int *enabled);

		int SystemReboot();
		int SystemShutdown();

		int Version(std::string *str);
		std::string Version();

		int Ping();
		int Quit();
		
		int Log(const std::string Level, const std::string Message);
		int LogDebug(const std::string Message);
		int LogInfo(const std::string Message);
		int LogNotice(const std::string Message);
		int LogWarning(const std::string Message);
		int LogError(const std::string Message);
		int LogCritical(const std::string Message);
		int LogAlert(const std::string Message);
		int LogEmergency(const std::string Message);
		

	private:
		ClientBase *m_Client;

};


