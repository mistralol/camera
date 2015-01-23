
class CameraHandler
{
	public:
		CameraHandler();
		~CameraHandler();

		void Init(const std::string Platform, const std::string CfgFile);
		bool ConfigLoad(Json::Value &);
		bool ConfigSave(Json::Value &);

		int RTSPGetClientCount(int *value);

		int RTSPSetMaxClients(int max);
		int RTSPGetMaxClients(int *max);

		int RTSPSetMaxBacklog(int max);
		int RTSPGetMaxBacklog(int *max);

		void Wait();
		void Quit();

	private:
		Barrier m_QuitBarrier;
		PlatformBase *m_Platform;
		RTSPServer *m_RServer;
		Config *m_Config;
		std::string m_CfgFile;

		//Video Stuff
		std::map<unsigned int, struct VideoStreamConfig *> m_VideoStreams;

};


