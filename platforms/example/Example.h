
#define GPIO_OUTPUTS 5

class Example : public PlatformBase
{
	public:
		Example();
		~Example();

		bool ConfigLoad(Json::Value &);
		bool ConfigSave(Json::Value &);

		unsigned int VideoInputCount();
		bool VideoInputSupportedInfo(unsigned int stream, VideoInputSupported *info);
		void VideoInputDefaultConfig(unsigned int stream, VideoInputConfig *config);
		bool VideoInputPipeline(unsigned int input, VideoInputConfig *config, std::string *str);

		unsigned int VideoOutputCount();
		void VideoOutputSupportedInfo(unsigned int output, VideoOutputSupported *info);

		unsigned int GPIOOutputCount();
		void GPIOOutputSetState(unsigned int output, bool enabled);
		bool GPIOOutputGetState(unsigned int output);

	private:
		bool m_gpio_outputs[GPIO_OUTPUTS];


};


