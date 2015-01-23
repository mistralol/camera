
class CameraHandler
{
	public:
		CameraHandler();
		~CameraHandler();

		void Init(const std::string Platform, const std::string CfgFile);
		bool ConfigLoad(Json::Value &);
		bool ConfigSave(Json::Value &);

		void Wait();
		void Quit();


		RTSPServer *RServer;

	private:
		Barrier m_QuitBarrier;
		PlatformBase *m_Platform;
		
		Config *m_Config;
		std::string m_CfgFile;

		//Video Stuff
		std::map<unsigned int, struct VideoStreamConfig *> m_VideoStreams;

};


