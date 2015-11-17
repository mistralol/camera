#include <string>
#include <sstream>
#include <json/json.h>

#include <VideoOutputConfig.h>

VideoOutputConfig::VideoOutputConfig()
{

}

VideoOutputConfig::~VideoOutputConfig()
{

}


std::string VideoOutputConfig::ToStr() const
{
	std::stringstream ss;
	ss << "FIXME";
	return ss.str();
}

bool VideoOutputConfig::ConfigSave(Json::Value &json)
{
	return true;
}

bool VideoOutputConfig::ConfigLoad(Json::Value &json)
{
	return true;
}

