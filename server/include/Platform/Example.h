
class Example : public PlatformBase
{
	public:
		Example();
		~Example();

		bool Init();

		bool ConfigLoad(Json::Value &);
		bool ConfigSave(Json::Value &);

		unsigned int VideoStreamCount();

		bool VideoStreamSupportedInfo(unsigned int stream, VideoStreamSupported *info);

		void VideoStreamDefaultConfig(unsigned int stream, VideoStreamConfig *config);
		bool VideoStreamConfigure(unsigned int stream, const VideoStreamConfig *config);

		bool VideoStreamEnable(unsigned int stream);
		bool VideoStreamDisable(unsigned int stream);

	private:
		std::map<unsigned int, PipelineBasic *> m_videopipelines;
		std::map<unsigned int, VideoStreamConfig> m_videoconfig;


};


