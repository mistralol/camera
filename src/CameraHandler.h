
class CameraHandler
{
	public:
		CameraHandler();
		~CameraHandler();

		void Init(const std::string WebRoot, const std::string Platform, const std::string CfgFile);
		bool ConfigLoad(Json::Value &);
		bool ConfigSave(Json::Value &);

		void VideoInputCount(int *count);

		bool VideoInputSetEnabled(unsigned int input, bool enabled);
		bool VideoInputGetEnabled(unsigned int input, bool &enabled);
		int VideoInputSetConfig(unsigned int input, VideoInputConfig *cfg);
		int VideoInputGetConfig(unsigned int input, VideoInputConfig *cfg);
		int VideoInputGetSupported(unsigned int input, VideoInputSupported *info);

		bool VideoInputEnable(unsigned int input);
		bool VideoInputDisable(unsigned int input);

		void Wait();
		void Quit();


		WebServer *WServer;
		RTSPServer *RServer;
		Config *Cfg;

	private:
		void *m_dll;
		Barrier m_QuitBarrier;

		GstLogger *m_gstlogger;

		PlatformBase *m_Platform;
	
		//Config Stuff
		std::string m_CfgFile;
		Mutex m_ConfigMutex;

		//Video Input Stuff
		Mutex m_VideoInputMutex;
		std::map<unsigned int, struct VideoInputConfig *> m_VideoInputs;

};


