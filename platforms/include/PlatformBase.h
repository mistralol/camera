

class PlatformBase
{
	public:
		virtual ~PlatformBase()
		{
		}

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

		virtual unsigned int VideoInputCount()
		{
			LogWarning("Platform has not implemented function %s", __FUNCTION__);
			return 0;
		}
		
		virtual bool VideoInputSupportedInfo(unsigned int input, VideoInputSupported *info)
		{
			LogWarning("Platform has not implemented function %s", __FUNCTION__);
			return false;
		}

		virtual void VideoInputDefaultConfig(unsigned int input, VideoInputConfig *config)
		{
			LogCritical("Platform has not implemented function %s", __FUNCTION__);
			abort();
		}
		
		
		virtual bool VideoInputConfigure(unsigned int input, const VideoInputConfig *config)
		{
			LogWarning("Platform has not implemented function %s", __FUNCTION__);
			return false;
		}

		virtual bool VideoInputEnable(unsigned int input)
		{
			LogWarning("Platform has not implemented function %s", __FUNCTION__);
			return false;
		}
		
		virtual bool VideoInputDisable(unsigned int input)
		{
			LogWarning("Platform has not implemented function %s", __FUNCTION__);
			return false;
		}
		
		virtual bool VideoInputRestart(unsigned int input)
		{
			if (VideoInputDisable(input) == false)
			{
				LogCritical("PlatformBase::VideoInputRestart - VideoInputDisable Failed!!");
				return false;
			}

			if (VideoInputEnable(input) == false)
			{
				LogCritical("PlatformBase::VideoInputRestart - VideoInputDisable Failed!!");
				return false;
			}
			return true;
		}

	protected:



};

