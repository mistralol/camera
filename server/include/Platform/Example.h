
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
};


