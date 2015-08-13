
class CameraServer : public IServerHandler
{
	public:
		CameraServer();
		~CameraServer();

		void Init(const std::string Platform, const std::string CfgFile);

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

		static int UserCreate(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserAuth(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserDelete(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserExists(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserSetPassword(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int UserList(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
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
		
		static int DebugGetEnabled(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int DebugSetEnabled(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);

		static int Version(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int Ping(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int Quit(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int Log(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);

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


