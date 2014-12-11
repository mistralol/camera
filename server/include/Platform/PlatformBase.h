

class PlatformBase
{
	public:
		virtual ~PlatformBase();

		virtual bool Init();

		/* Video Stream Specific */

		virtual bool GetVideoSupported(struct VideoSupported *);
		virtual bool GetVideoStreamSupported(unsigned int Stream, struct VideoStreamSupported *);

		virtual bool GetVideoStreamDefaultConfig(unsigned int Stream, struct ConfigVideoStream *);
		virtual bool GetVideoStreamConfig(unsigned int Stream, struct ConfigVideoStream *);
		virtual bool SetVideoStreamConfig(unsigned int Stream, struct ConfigVideoStream *);
		virtual bool VideoStreamEnable(unsigned int Stream);
		virtual bool VideoStreamDisable(unsigned int Stream);

		/* Audio Stream Specific */

		virtual bool GetAudioSupported(struct AudioSupported *);
		virtual bool GetAudioStreamSupported(unsigned int Stream, struct AudioStreamSupported *);

		/* Text Stream Specific */
		virtual bool GetTextSupported(struct TextSupported *);
		virtual bool GetTextStreamSupported(unsigned int Stream, struct TextStreamSupported *);

};

