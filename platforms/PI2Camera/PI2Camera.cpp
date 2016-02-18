
#include <Camera.h>

#include <PI2Camera.h>

extern "C" PlatformBase *Create()
{
	return new PI2Camera();
}

PI2Camera::PI2Camera()
{
	LogDebug("PI2Camera::PI2Camera");
}

PI2Camera::~PI2Camera()
{

}

bool PI2Camera::ConfigLoad(Json::Value &json)
{
	return true;
}

bool PI2Camera::ConfigSave(Json::Value &json)
{
	return true;
}

unsigned int PI2Camera::VideoInputCount()
{
	LogDebug("PI2Camera::VideoInputCount");
	return 1;
}

bool PI2Camera::VideoInputSupportedInfo(unsigned int input, VideoInputSupported *info)
{
	LogDebug("PI2Camera::GetVideoInputSupported");
	info->Clear();
	info->AddCodec("H264", "128x96", 1, 30);
	info->AddCodec("H264", "176x144", 1, 30);
	info->AddCodec("H264", "320x200", 1, 30);
	info->AddCodec("H264", "352x288", 1, 30);
	info->AddCodec("H264", "640x400", 1, 30);
	info->AddCodec("H264", "704x576", 1, 30);
	info->AddCodec("H264", "720x480", 1, 30);
	info->AddCodec("H264", "1280x720", 1, 30);
	
	info->hflip = 1;
	info->vflip = 1;

	return true;
}

void PI2Camera::VideoInputDefaultConfig(unsigned int input, VideoInputConfig *config)
{
	switch(input)
	{
		default:
			config->SetCodec("H264");
			config->SetResolution("640x400");
			config->SetFrameRate(30);
			config->SetEnabled(true);
			break;
	}
}

bool PI2Camera::VideoInputPipeline(unsigned int input, VideoInputConfig *config, std::string *str)
{
	std::stringstream pipe;
	std::string res = config->GetResolution();
	std::string width = "";
	std::string height = "";

	if (String::SplitOne(&res, &width, &height, "x") == false)
	{
		LogError("Example::VideoInputEnable - Failed to parse: %s", res.c_str());
		return false;
	}

	if (config->GetCodec() == "H264")
	{
		pipe << "rpicamsrc preview=false keyframe-interval=" << config->GetFrameRate();
		if (config->hflip)
			pipe << " hflip=true";
		if (config->vflip)
			pipe << " vflip=true";
		pipe << " ! capsfilter caps=capsfilter caps=\"video/x-h264, framerate=" << config->GetFrameRate() << "/1";
		pipe << ", width=" << width << " , height=" << height << "\"";
		pipe << " ! h264parse";
		pipe << " ! video/x-h264, stream-format=avc, alignment=au";
		pipe << " ! internalsink streamname=video" << input;
	}
	else
	{
		LogError("PI2Camera::VideoInputEnable - Unknown Codec");
		return false;
	}

	*str = pipe.str();
	return true;
}




