

class PlatformBase
{
	public:
		virtual ~PlatformBase()
		{
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

		//Required
		virtual unsigned int VideoInputCount()
		{
			LogWarning("Platform has not implemented function %s", __FUNCTION__);
			return 0;
		}
		
		//Required
		virtual bool VideoInputSupportedInfo(unsigned int input, VideoInputSupported *info)
		{
			LogWarning("Platform has not implemented function %s", __FUNCTION__);
			return false;
		}

		//Required
		virtual void VideoInputDefaultConfig(unsigned int input, VideoInputConfig *config)
		{
			LogCritical("Platform has not implemented function %s", __FUNCTION__);
			abort();
		}

		//Required
		virtual bool VideoInputPipeline(unsigned int input, VideoInputConfig *config, std::string *str)
		{
			LogCritical("Platform has not implemented function %s", __FUNCTION__);
			abort();
			return false;
		}
		
		//If required to overload the pipeline implement all the functions		
		virtual bool VideoInputConfigure(unsigned int input, const VideoInputConfig *config)
		{
			LogDebug("PlatformBase::VideoInputConfigure(%u, \"%s\")", input, config->ToStr().c_str());
			m_videoinputconfig[input] = *config; //Take a copy
			return true;
		}

		virtual bool VideoInputEnable(unsigned int input)
		{
			std::stringstream pipename;
			pipename << "Video Input: " << input;

			LogDebug("PlatformBase::VideoInputEnable(%u)", input);
			if (m_videoinputconfig.find(input) == m_videoinputconfig.end())
				abort(); //Bug because VideoInputEnable is called before VideoInputConfigure
			if (m_videoinputpipelines.find(input) != m_videoinputpipelines.end())
				abort(); //WTF? We already have a pipeline which means the input is enabled

			std::string pipe = "";
			if (VideoInputPipeline(input, &m_videoinputconfig[input], &pipe) == false)
			{
				LogCritical("PlatformBase::VideoInputEnable Failed to return pipeline");
				abort();
			}

			LogDebug("Example::VideoInputEnable - Pipeline %s", pipe.c_str());
			LogInfo("Example::VideoInputEnable - Starting Input %u", input);
			PipelineBasic *pipeline = new PipelineBasic(pipename.str(), pipe);
			m_videoinputpipelines[input] = pipeline;
			m_videoinputpipelines[input]->Start();
			return true;
		}
		
		virtual bool VideoInputDisable(unsigned int input)
		{
			LogDebug("PlatformBase::VideoInputDisable(%u)", input);
			std::map<unsigned int, PipelineBasic *>::iterator it = m_videoinputpipelines.find(input);
			if (it == m_videoinputpipelines.end())
			{
				LogError("PlatformBase::VideoInputDisable Failed because there is no pipeline");
				return false;
			}
			PipelineBasic *pipeline = it->second;
			m_videoinputpipelines.erase(it);
			LogInfo("PlatformBase::VideoInputDisable - Stopping Input %u", input);
			pipeline->Stop();
			delete pipeline;
			return true;
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

		/* Video Output Stuff */

		virtual unsigned int VideoOutputCount()
		{
			LogWarning("Platform has not implemented function %s", __FUNCTION__);
			return 0;
		}
		
		virtual void VideoOutputSupportedInfo(unsigned int output, VideoOutputSupported *info)
		{
			LogCritical("Platform has not implemented function %s", __FUNCTION__);
			abort();
		}
		
		virtual bool VideoOutputConfigure(unsigned int output, VideoOutputConfig *info)
		{
			LogCritical("Platform has not implemented function %s", __FUNCTION__);
			abort();
		}
		
		virtual bool VideoOutputEnable(unsigned int output)
		{
			LogCritical("Platform has not implemented function %s", __FUNCTION__);
			abort();
		}
		
		virtual bool VideoOutputDisable(unsigned int output)
		{
			LogCritical("Platform has not implemented function %s", __FUNCTION__);
			abort();
		}

		virtual bool VideoOutputRestart(unsigned int output)
		{
			if (VideoOutputEnable(output) == false)
			{
				LogCritical("PlatformBase::VideoOutputRestart - VideoOutputDisable Failed!!");
				return false;
			}

			if (VideoOutputDisable(output) == false)
			{
				LogCritical("PlatformBase::VideoOutputRestart - VideoOutputDisable Failed!!");
				return false;
			}
			return true;
		}
		
		virtual unsigned int AudioInputCount()
		{
			LogWarning("Platform has not implemented function %s", __FUNCTION__);
			return 0;
		}
		
		virtual unsigned int AudioOutputCount()
		{
			LogWarning("Platform has not implemented function %s", __FUNCTION__);
			return 0;
		}
		
		virtual unsigned int GPIOInputCount()
		{
			LogWarning("Platform has not implemented function %s", __FUNCTION__);
			return 0;
		}
		
		virtual std::string GPIOInputDesc(unsigned int input)
		{
			LogCritical("Platform has not implemented function %s", __FUNCTION__);
			abort();
		}
		
		virtual bool GPIOInputGetState(unsigned int input)
		{
			LogCritical("Platform has not implemented function %s", __FUNCTION__);
			abort();
		}
		
		virtual bool GPIOInputEnable(unsigned int input)
		{
			LogCritical("Platform has not implemented function %s", __FUNCTION__);
			abort();
		}
		
		virtual bool GPIOInputDisable(unsigned int input)
		{
			LogCritical("Platform has not implemented function %s", __FUNCTION__);
			abort();
		}
		
		virtual unsigned int GPIOOutputCount()
		{
			LogWarning("Platform has not implemented function %s", __FUNCTION__);
			return 0;
		}
		
		virtual void GPIOOutputSetState(unsigned int output, bool enabled)
		{
			LogCritical("Platform has not implemented function %s", __FUNCTION__);
			abort();
		}
		
		virtual bool GPIOOutputGetState(unsigned int output)
		{
			LogCritical("Platform has not implemented function %s", __FUNCTION__);
			abort();
		}

	protected:


	private:
		//Video Input
		std::map<unsigned int, PipelineBasic *> m_videoinputpipelines;
		std::map<unsigned int, VideoInputConfig> m_videoinputconfig;

};

