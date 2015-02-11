
class CameraServer : public IServerHandler
{
	public:
		CameraServer();
		~CameraServer();

		void Init(const std::string Platform, const std::string CfgFile);

		void Wait();
		void Quit();

		static int RTSPGetPort(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int RTSPSetPort(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int RTSPGetClientCount(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int RTSPSetMaxClients(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int RTSPGetMaxClients(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int RTSPSetMaxBacklog(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int RTSPGetMaxBacklog(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);

		static int VideoStreamCount(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);

		static int VideoStreamSetEnabled(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int VideoStreamGetEnabled(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);

		static int Version(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int Ping(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int Quit(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);

		void OnPreNewConnection();

		void OnPostNewConnection(IServerConnection *Connection);		
		void OnDisconnect(IServerConnection *Connection);
		
		int OnRequest(IServerConnection *Connection, Request *request, Request *response);

		int OnCommand(IServerConnection *Connection, Request *request);
		void OnBadLine(IServerConnection *Connection, const std::string *line);

	private:
		CameraHandler *m_handler;

};


