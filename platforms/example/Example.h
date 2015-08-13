
class Example : public PlatformBase
{
	public:
		Example();
		~Example();

		bool Init();

		bool ConfigLoad(Json::Value &);
		bool ConfigSave(Json::Value &);

		unsigned int VideoInputCount();

		bool VideoInputSupportedInfo(unsigned int stream, VideoInputSupported *info);

		void VideoInputDefaultConfig(unsigned int stream, VideoInputConfig *config);
		bool VideoInputConfigure(unsigned int stream, const VideoInputConfig *config);

		bool VideoInputEnable(unsigned int stream);
		bool VideoInputDisable(unsigned int stream);

	private:
		std::map<unsigned int, PipelineBasic *> m_videoinputpipelines;
		std::map<unsigned int, VideoInputConfig> m_videoinputconfig;


};


