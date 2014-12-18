
class CameraHandler : public IServerHandler
{
	public:
		CameraHandler();
		~CameraHandler();


		void Init(const std::string Platform, const std::string CfgFile);
		bool ConfigLoad(Json::Value *);
		bool ConfigSave(Json::Value *);

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
		PlatformBase *m_Platform;
		RTSPServer *m_RServer;
		Config *m_Config;
		std::string m_CfgFile;

		//Video Stuff
		std::map<unsigned int, struct ConfigVideoStream *> m_VideoStreams;

};


