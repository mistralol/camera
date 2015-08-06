

class PlatformBase
{
	public:
		virtual ~PlatformBase();

		virtual bool Init()
		{
			LogCritical("PlatformBase::Init is not implemented");
			return false;
		}

		virtual bool ConfigLoad(Json::Value &)
		{
			LogWarning("Platform has not implemented function %s", __FUNCTION__);
			return false;
		}
		
		virtual bool ConfigSave(Json::Value &)
		{
			LogWarning("Platform has not implemented function %s", __FUNCTION__);
			return false;
		}

		/* Video Stream Specific */

		virtual unsigned int VideoStreamCount()
		{
			LogWarning("Platform has not implemented function %s", __FUNCTION__);
			return 0;
		}
		
		virtual bool VideoStreamSupportedInfo(unsigned int stream, VideoStreamSupported *info)
		{
			LogWarning("Platform has not implemented function %s", __FUNCTION__);
			return false;
		}

		virtual void VideoStreamDefaultConfig(unsigned int stream, VideoStreamConfig *config)
		{
			LogCritical("Platform has not implemented function %s", __FUNCTION__);
			abort();
		}
		
		
		virtual bool VideoStreamConfigure(unsigned int stream, const VideoStreamConfig *config)
		{
			LogWarning("Platform has not implemented function %s", __FUNCTION__);
			return false;
		}

		virtual bool VideoStreamEnable(unsigned int stream)
		{
			LogWarning("Platform has not implemented function %s", __FUNCTION__);
			return false;
		}
		
		virtual bool VideoStreamDisable(unsigned int stream)
		{
			LogWarning("Platform has not implemented function %s", __FUNCTION__);
			return false;
		}
		
		virtual bool VideoStreamRestart(unsigned int stream)
		{
			if (VideoStreamDisable(stream) == false)
			{
				LogCritical("PlatformBase::VideoStreamRestart - VideoStreamDisable Failed!!");
				return false;
			}

			if (VideoStreamEnable(stream) == false)
			{
				LogCritical("PlatformBase::VideoStreamRestart - VideoStreamDisable Failed!!");
				return false;
			}
			return true;
		}

		/* Audio Stream Specific */

		virtual unsigned int GetAudioNumStreams()
		{
			LogWarning("Platform has not implemented function %s", __FUNCTION__);
			return 0;
		}

	protected:



};

