
class Example : public PlatformBase
{
	public:
		Example();
		~Example();

		bool Init();

		bool ConfigLoad(Json::Value &);
		bool ConfigSave(Json::Value &);

		unsigned int GetVideoNumStreams();

		bool GetVideoStreamSupported(unsigned int stream, VideoStreamSupported *info);
};


