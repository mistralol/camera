

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

		virtual void VideoStreamDefaultConfig(unsigned int stream, VideoStreamConfig *config);
		virtual bool VideoStreamConfigure(unsigned int stream, const VideoStreamConfig *config);

		virtual bool VideoStreamEnable(unsigned int stream);
		virtual bool VideoStreamDisable(unsigned int stream);
		virtual bool VideoStreamRestart(unsigned int stream);

		/* Audio Stream Specific */

		virtual unsigned int GetAudioNumStreams();

	protected:



};

