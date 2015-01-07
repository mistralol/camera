

class PlatformBase
{
	public:
		virtual ~PlatformBase();

		virtual bool Init();

		virtual bool ConfigLoad(Json::Value &);
		virtual bool ConfigSave(Json::Value &);

		/* Video Stream Specific */

		virtual unsigned int GetVideoNumStreams();
		virtual bool GetVideoStreamSupported(unsigned int stream, VideoStreamSupported *info);

		/* Audio Stream Specific */

		virtual unsigned int GetAudioNumStreams();

	protected:



};

