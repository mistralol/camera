
#include <Camera.h>

Example::Example()
{

}

Example::~Example()
{

}

bool Example::Init()
{
	LogDebug("Example::Init");

	return true;
}

bool Example::ConfigLoad(Json::Value &json)
{
	return false;
}

bool Example::ConfigSave(Json::Value &json)
{
	return false;
}

unsigned int Example::GetVideoNumStreams()
{
	LogDebug("Example::GetVideoNumStreams");
	return 1;
}

bool Example::GetVideoStreamSupported(unsigned int stream, VideoStreamSupported *info)
{
	LogDebug("Example::GetVideoStreamSupported");
	info->Clear();
	info->AddCodec("H264", "128x96", 1, 30);
	info->AddCodec("H264", "176x144", 1, 30);
	info->AddCodec("H264", "320x280", 1, 30);
	info->AddCodec("H264", "352x288", 1, 30);
	info->AddCodec("H264", "640x400", 1, 30);
	info->AddCodec("H264", "704x576", 1, 30);
	info->AddCodec("H264", "720x480", 1, 30);
	info->AddCodec("H264", "1280x1080", 1, 30);
	return true;
}


