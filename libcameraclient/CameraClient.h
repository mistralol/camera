
#include <CameraClientException.h>
#include <StatsInfo.h>

class CameraClient
{
	public:
		CameraClient();
		~CameraClient();

		bool Connect(const std::string ConnectionString);
		void Disconnect();

		void RTSPKickAll();

		int RTSPGetPort();
		void RTSPSetPort(int value);

		int RTSPGetClientCount();

		void RTSPSetMaxClients(int max);
		int RTSPGetMaxClients();

		void RTSPSetMaxBacklog(int max);
		int RTSPGetMaxBacklog();

		int VideoInputCount();
		void VideoInputSetEnabled(int input, bool enabled);
		int VideoInputGetEnabled(int input);
		VideoInputConfig VideoInputGetConfig(int input);
		void VideoInputSetConfig(int input, VideoInputConfig *cfg);
		VideoInputSupported VideoInputGetSupported(int input);
		
		int VideoOutputCount();
		VideoOutputSupported VideoOutputGetSupported(int input);
		
		std::vector<std::string> VideoOutputTourList();
		void VideoOutputTourAdd(VideoOutputTour *tour);
		void VideoOutputTourUpdate(VideoOutputTour *tour);
		VideoOutputTour VideoOutputTourGet(const std::string &name);
		bool VideoOutputTourExists(const std::string &name);
		void VideoOutputTourRemove(const std::string &name);
		
		int GPIOOutputCount();
		void GPIOOutputSetState(int output, bool enabled);
		void GPIOOutputSetState(int output, bool enabled, const struct timespec *ts);
		bool GPIOOutputGetState(int output);
		
		void UserCreate(const std::string Username, const std::string Password, const std::string EMail);
		bool UserAuth(const std::string Username, const std::string Password);
		void UserDelete(const std::string Username);
		bool UserExists(const std::string Username);
		void UserTouch(const std::string Username);
		bool UserIsLockedOut(const std::string Username);
		bool UserIsApproved(const std::string Username);
		bool UserIsOnline(const std::string Username);
		void UserSetLockedOut(const std::string Username, bool value);
		void UserSetApproved(const std::string Username, bool value);
		void UserSetPassword(const std::string Username, const std::string Password);
		int UserGetLockoutDuration();
		void UserSetLockoutDuration(int value);
		int UserGetMaxFailedAttempts();
		void UserSetMaxFailedAttempts(int value);
		int UserGetAutoLogOff();
		void UserSetAutoLogOff(int value);
		void UserInfo(const std::string Username, struct UserItem *);
		std::vector<std::string> UserList();
		std::string UserGetUserFromEMail(const std::string EMail);
		std::string UserGetUserFromKey(const std::string Key);

		
		void GroupCreate(const std::string Group);
		void GroupDelete(const std::string Group);
		bool GroupExists(const std::string Group);
		bool GroupIsUserInGroup(const std::string Group, const std::string User);
		void GroupUserAdd(const std::string Group, const std::string User);
		void GroupUserRemove(const std::string Group, const std::string User);
		std::vector<std::string> GroupList();
		std::vector<std::string> GroupListUsers(const std::string Group);

		int WebServerGetPort();
		void WebServerSetPort(int port);
		bool WebServerGetEnabled();
		void WebServerSetEnabled(bool enabled);
		std::string WebServerGetProperty(const std::string key, const std::string def);
		void WebServerSetProperty(const std::string key, const std::string value);
		void WebServerRestart();
		
		int WebStreamStart(WebStreamOptions *);
		
		void DebugSetEnabled(bool enabled);
		bool DebugGetEnabled();

		void SystemReboot();
		void SystemShutdown();

		std::string Version();

		void Ping();
		void Quit();
		
		void Log(const std::string Level, const std::string Message);
		void LogDebug(const std::string Message);
		void LogInfo(const std::string Message);
		void LogNotice(const std::string Message);
		void LogWarning(const std::string Message);
		void LogError(const std::string Message);
		void LogCritical(const std::string Message);
		void LogAlert(const std::string Message);
		void LogEmergency(const std::string Message);
		
		std::vector<std::string> StatsList();
		void StatsInfo(const std::string key, struct StatsInfo *);
		void StatsDump();
		void StatsReset();

	private:
		ClientBase *m_Client;

};


