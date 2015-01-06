
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

