
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

void WebStreamOptions::Encode(Json::Value &options)
{
	options["vinput"] = vinput;
	options["type"] = type;
	options["localonly"] = localonly;
	options["timeout"] = timeout;

	options["width"] = width;
	options["height"] = height;

	options["mjpeg_fps"] = mjpeg_fps;
	options["mjpeg_quality"] = mjpeg_quality;

}

void WebStreamOptions::Decode(const Json::Value &options)
{
	//FIXME: Checks
	vinput = options["vinput"].asInt();
	type = (WebStreamType) options["type"].asInt();
	localonly = options["localonly"].asBool();
	timeout = options["timeout"].asInt();
	
	width = options["width"].asInt();
	height = options["height"].asInt();

	mjpeg_fps = options["mjpeg_fps"].asInt();
	mjpeg_quality = options["mjpeg_quality"].asInt();
		
	return true;
}

