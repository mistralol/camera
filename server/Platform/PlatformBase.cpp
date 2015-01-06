
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
	return false;
}

bool PlatformBase::ConfigSave(Json::Value &)
{
	return false;
}

bool PlatformBase::GetVideoSupported(struct VideoSupported *)
{
	return false;
}

bool PlatformBase::GetVideoStreamSupported(unsigned int Stream, struct VideoStreamSupported *)
{
	return false;
}

bool PlatformBase::GetVideoStreamDefaultConfig(unsigned int Stream, struct ConfigVideoStream *)
{
	return false;
}

bool PlatformBase::GetVideoStreamConfig(unsigned int Stream, struct ConfigVideoStream *)
{
	return false;
}

bool PlatformBase::SetVideoStreamConfig(unsigned int Stream, struct ConfigVideoStream *)
{
	return false;
}

bool PlatformBase::VideoStreamEnable(unsigned int Stream)
{
	return false;
}

bool PlatformBase::VideoStreamDisable(unsigned int Stream)
{
	return false;
}

bool PlatformBase::GetAudioSupported(struct AudioSupported *)
{
	return false;
}

bool PlatformBase::GetAudioStreamSupported(unsigned int Stream, struct AudioStreamSupported *)
{
	return false;
}

bool PlatformBase::GetTextSupported(struct TextSupported *)
{
	return false;
}

bool PlatformBase::GetTextStreamSupported(unsigned int Stream, struct TextStreamSupported *)
{
	return false;
}

