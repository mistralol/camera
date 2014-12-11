
class CameraHandler : public IServerHandler
{
	public:
		CameraHandler();
		~CameraHandler();


		void Init(const std::string Platform, const std::string CfgFile);
		bool ConfigLoad();
		bool ConfigSave();

		void Wait();
		void Quit();


		void OnPreNewConnection();

		void OnPostNewConnection(IServerConnection *Connection);		
		void OnDisconnect(IServerConnection *Connection);
		
		int OnRequest(IServerConnection *Connection, Request *request, Request *response);

		int OnCommand(IServerConnection *Connection, Request *request);
		void OnBadLine(IServerConnection *Connection, const std::string *line);

	private:
		Barrier m_QuitBarrier;
		PlatformBase *m_Platform = NULL;
		RTSPServer *m_RServer = NULL;
		std::string m_CfgFile;

};


