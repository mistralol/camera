

class PI2Camera : public PlatformBase
{
	public:
		PI2Camera();
		~PI2Camera();

		bool ConfigLoad(Json::Value &);
		bool ConfigSave(Json::Value &);

		unsigned int VideoInputCount();
		bool VideoInputSupportedInfo(unsigned int stream, VideoInputSupported *info);
		void VideoInputDefaultConfig(unsigned int stream, VideoInputConfig *config);
		bool VideoInputPipeline(unsigned int input, VideoInputConfig *config, std::string *str);

	private:
		std::map<unsigned int, PipelineBasic *> m_videoinputpipelines;
		std::map<unsigned int, VideoInputConfig> m_videoinputconfig;
};




