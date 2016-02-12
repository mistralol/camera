
#include <string>
#include <sstream>
#include <json/json.h>

#include <stdio.h>

#include <WebStreamType.h>
#include <WebStreamOptions.h>

WebStreamOptions::WebStreamOptions() :
	vinput(0), type(Unknown), localonly(true), timeout(60),
	mjpeg_fps(0), mjpeg_quality(0)
{

}

bool WebStreamOptions::Encode(std::string &str)
{
	Json::Value json;
	
	json["vinput"] = vinput;
	json["type"] = type;
	json["localonly"] = localonly;
	json["timeout"] = timeout;

	json["width"] = width;
	json["height"] = height;

	json["mjpeg_fps"] = mjpeg_fps;
	json["mjpeg_quality"] = mjpeg_quality;
	
	std::stringstream ss;
	Json::StyledWriter styledWriter;
	ss << styledWriter.write(json);
	str = ss.str();
	return true;
}

bool WebStreamOptions::Decode(const std::string &str)
{
	Json::Value root;
	Json::Reader reader;

        if (reader.parse(str, root ) == false)
		return false;
		
	vinput = root["vinput"].asInt();
	type = (WebStreamType) root["type"].asInt();
	localonly = root["localonly"].asBool();
	timeout = root["timeout"].asInt();
	
	width = root["width"].asInt();
	height = root["height"].asInt();

	mjpeg_fps = root["mjpeg_fps"].asInt();
	mjpeg_quality = root["mjpeg_quality"].asInt();

		
	return true;
}

