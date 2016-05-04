
class CameraServer : public IServerHandler
{
	public:
		CameraServer();
		~CameraServer();

		void Init(const std::string WebRoot, const std::string Platform, const std::string CfgFile);

		void Wait();
		void Quit();

		static int VideoInputCount(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int VideoInputSetEnabled(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int VideoInputGetEnabled(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int VideoInputGetConfig(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int VideoInputSetConfig(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int VideoInputGetSupported(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);

		static int UserCreate(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int UserAuth(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int UserDelete(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int UserExists(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int UserTouch(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int UserIsLockedOut(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int UserIsApproved(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int UserIsOnline(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int UserSetLockedOut(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int UserSetApproved(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int UserSetPassword(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int UserGetLockoutDuration(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int UserSetLockoutDuration(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int UserGetMaxFailedAttempts(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int UserSetMaxFailedAttempts(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int UserGetAutoLogOff(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int UserSetAutoLogOff(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int UserInfo(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int UserList(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);

		static int WebServerGetPort(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int WebServerSetPort(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int WebServerGetEnabled(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int WebServerSetEnabled(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int WebServerGetProperty(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int WebServerSetProperty(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int WebServerRestart(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		
		static int WebStreamStart(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		
		static int DebugGetEnabled(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int DebugSetEnabled(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);

		static int Version(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int Ping(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int Quit(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int Log(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);

		static int StatsList(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int StatsInfo(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int StatsDump(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int StatsReset(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		
		static void StatsDump();
		static void StatsReset();

		void OnPreNewConnection();

		void OnPostNewConnection(IServerConnection *Connection);		
		void OnDisconnect(IServerConnection *Connection);
		
		int OnRequest(IServerConnection *Connection, Json::Value &request, Json::Value &response);

		int OnCommand(IServerConnection *Connection, Json::Value &request);
		void OnBadLine(IServerConnection *Connection, const std::string *line);

	private:
		CameraHandler *m_handler;

};


