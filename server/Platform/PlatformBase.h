

class PlatformBase
{
	public:
		virtual ~PlatformBase();

		virtual bool Init();

		/* Video Stream Specific */

		virtual bool GetVideoSupported(struct VideoSupported *);
		virtual bool GetVideoStreamSupported(int Stream, struct VideoStreamSupported *);

		virtual bool GetVideoStreamConfig(int Stream, struct ConfigVideoStream *);
		virtual bool SetVideoStreamConfig(int Stream, struct ConfigVideoStream *);
		virtual bool VideoStreamEnable(int Stream);
		virtual bool VideoStreamDisable(int Stream);

		/* Audio Stream Specific */

		virtual bool GetAudioSupported(struct AudioSupported *);
		virtual bool GetAudioStreamSupported(int Stream, struct AudioStreamSupported *);

		/* Text Stream Specific */
		virtual bool GetTextSupported(struct TextSupported *);
		virtual bool GetTextStreamSupported(int Stream, struct TextStreamSupported *);

};

