
#include <Camera.h>

PlatformBase::~PlatformBase()
{

}

bool PlatformBase::Init()
{
	return false;
}

bool PlatformBase::ConfigLoad(Json::Value &)
{
	LogWarning("Platform has not implemented function %s", __FUNCTION__);
	return false;
}

bool PlatformBase::ConfigSave(Json::Value &)
{
	LogWarning("Platform has not implemented function %s", __FUNCTION__);
	return false;
}

unsigned int PlatformBase::VideoStreamCount()
{
	LogWarning("Platform has not implemented function %s", __FUNCTION__);
	return 0;
}

bool PlatformBase::VideoStreamSupportedInfo(unsigned int stream, VideoStreamSupported *info)
{
	LogWarning("Platform has not implemented function %s", __FUNCTION__);
	return false;
}

void PlatformBase::VideoStreamDefaultConfig(unsigned int stream, VideoStreamConfig *config)
{
	LogCritical("Platform has not implemented function %s", __FUNCTION__);
	abort();
}

bool PlatformBase::VideoStreamConfigure(unsigned int stream, const VideoStreamConfig *config)
{
	LogWarning("Platform has not implemented function %s", __FUNCTION__);
	return false;
}

bool PlatformBase::VideoStreamEnable(unsigned int stream)
{
	LogWarning("Platform has not implemented function %s", __FUNCTION__);
	return false;
}

bool PlatformBase::VideoStreamDisable(unsigned int stream)
{
	LogWarning("Platform has not implemented function %s", __FUNCTION__);
	return false;
}

bool PlatformBase::VideoStreamRestart(unsigned int stream)
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


unsigned int PlatformBase::GetAudioNumStreams()
{
	LogWarning("Platform has not implemented function %s", __FUNCTION__);
	return 0;
}

