
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

unsigned int PlatformBase::GetVideoNumStreams()
{
	return 0;
}

unsigned int PlatformBase::GetAudioNumStreams()
{
	return 0;
}

