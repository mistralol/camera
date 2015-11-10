
class CameraServer : public IServerHandler
{
	public:
		CameraServer();
		~CameraServer();

		void Init(const std::string WebRoot, const std::string Platform, const std::string CfgFile);

		void Wait();
		void Quit();

		static int RTSPKickAll(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int RTSPGetPort(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int RTSPSetPort(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int RTSPGetClientCount(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int RTSPSetMaxClients(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int RTSPGetMaxClients(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int RTSPSetMaxBacklog(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int RTSPGetMaxBacklog(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);

		static int VideoInputCount(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int VideoInputSetEnabled(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int VideoInputGetEnabled(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int VideoInputGetConfig(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int VideoInputSetConfig(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int VideoInputGetSupported(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);

		static int VideoOutputCount(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int VideoOutputGetSupported(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int VideoOutputTourList(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int VideoOutputTourAdd(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int VideoOutputTourUpdate(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int VideoOutputTourGet(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int VideoOutputTourExists(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int VideoOutputTourRemove(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);

		static int GPIOOutputCount(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int GPIOOutputSetState(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int GPIOOutputGetState(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);

		static int UserCreate(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserAuth(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserDelete(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserExists(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserTouch(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserIsLockedOut(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserIsApproved(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserIsOnline(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserSetLockedOut(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserSetApproved(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserSetPassword(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserGetLockoutDuration(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserSetLockoutDuration(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserGetMaxFailedAttempts(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserSetMaxFailedAttempts(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserGetAutoLogOff(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserSetAutoLogOff(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserInfo(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserList(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserGetFromEMail(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserGetFromKey(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		
		static int GroupCreate(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int GroupDelete(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int GroupExists(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int GroupIsUserInGroup(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int GroupUserAdd(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int GroupUserRemove(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int GroupList(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int GroupListUsers(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);

		static int WebServerGetPort(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int WebServerSetPort(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int WebServerGetEnabled(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int WebServerSetEnabled(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int WebServerGetProperty(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int WebServerSetProperty(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int WebServerRestart(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		
		static int WebStreamStart(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		
		static int DebugGetEnabled(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int DebugSetEnabled(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);

		static int Version(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int Ping(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int Quit(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int Log(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);

		static int StatsList(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int StatsInfo(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int StatsDump(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int StatsReset(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		
		static void StatsDump();
		static void StatsReset();

		void OnPreNewConnection();

		void OnPostNewConnection(IServerConnection *Connection);		
		void OnDisconnect(IServerConnection *Connection);
		
		int OnRequest(IServerConnection *Connection, Request *request, Request *response);

		int OnCommand(IServerConnection *Connection, Request *request);
		void OnBadLine(IServerConnection *Connection, const std::string *line);

	private:
		CameraHandler *m_handler;

};


