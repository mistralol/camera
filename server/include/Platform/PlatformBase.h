

class PlatformBase
{
	public:
		virtual ~PlatformBase();

		virtual bool Init();

		virtual bool ConfigLoad(Json::Value &);
		virtual bool ConfigSave(Json::Value &);

		/* Video Stream Specific */

		virtual unsigned int VideoStreamCount();
		virtual bool VideoStreamSupportedInfo(unsigned int stream, VideoStreamSupported *info);

		virtual bool VideoStreamEnable(unsigned int stream);
		virtual bool VideoStreamDisable(unsigned int stream);

		/* Audio Stream Specific */

		virtual unsigned int GetAudioNumStreams();

	protected:



};

