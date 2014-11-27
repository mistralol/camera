
#include <Camera.h>

PlatformBase::~PlatformBase()
{

}

bool PlatformBase::Init()
{
	return false;
}

bool PlatformBase::GetVideoSupported(struct VideoSupported *)
{
	return false;
}

bool PlatformBase::GetVideoStreamSupported(int Stream, struct VideoStreamSupported *)
{
	return false;
}

bool PlatformBase::GetVideoStreamConfig(int Stream, struct ConfigVideoStream *)
{
	return false;
}

bool PlatformBase::SetVideoStreamConfig(int Stream, struct ConfigVideoStream *)
{
	return false;
}

bool PlatformBase::VideoStreamEnable(int Stream)
{
	return false;
}

bool PlatformBase::VideoStreamDisable(int Stream)
{
	return false;
}

bool PlatformBase::GetAudioSupported(struct AudioSupported *)
{
	return false;
}

bool PlatformBase::GetAudioStreamSupported(int Stream, struct AudioStreamSupported *)
{
	return false;
}

bool PlatformBase::GetTextSupported(struct TextSupported *)
{
	return false;
}

bool PlatformBase::GetTextStreamSupported(int Stream, struct TextStreamSupported *)
{
	return false;
}

