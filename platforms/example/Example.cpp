
#include <Camera.h>

#include <Example.h>

extern "C" PlatformBase *Create()
{
	return new Example();
}

Example::Example()
{
	LogDebug("Example::Example");

	for(int i=0;i<GPIO_OUTPUTS;i++)
		m_gpio_outputs[i] = false;
}

Example::~Example()
{

}

bool Example::ConfigLoad(Json::Value &json)
{
	return true;
}

bool Example::ConfigSave(Json::Value &json)
{
	return true;
}

unsigned int Example::VideoInputCount()
{
	LogDebug("Example::VideoInputCount");
	return 2;
}

bool Example::VideoInputSupportedInfo(unsigned int input, VideoInputSupported *info)
{
	LogDebug("Example::GetVideoInputSupported");
	info->Clear();
	info->AddCodec("H264", "128x96", 1, 30);
	info->AddCodec("H264", "176x144", 1, 30);
	info->AddCodec("H264", "320x280", 1, 30);
	info->AddCodec("H264", "352x288", 1, 30);
	info->AddCodec("H264", "640x400", 1, 30);
	info->AddCodec("H264", "704x576", 1, 30);
	info->AddCodec("H264", "720x480", 1, 30);
	info->AddCodec("H264", "1280x720", 1, 30);
	info->AddCodec("H264", "1920x1080", 1, 30);
	
	info->AddCodec("MJPEG", "128x96", 1, 30);
	info->AddCodec("MJPEG", "176x144", 1, 30);
	info->AddCodec("MJPEG", "320x280", 1, 30);
	info->AddCodec("MJPEG", "352x288", 1, 30);
	info->AddCodec("MJPEG", "640x400", 1, 30);
	info->AddCodec("MJPEG", "704x576", 1, 30);
	info->AddCodec("MJPEG", "720x480", 1, 30);
	info->AddCodec("MJPEG", "1280x720", 1, 30);
	info->AddCodec("MJPEG", "1920x1080", 1, 30);
	
	return true;
}

void Example::VideoInputDefaultConfig(unsigned int input, VideoInputConfig *config)
{
	switch(input)
	{
		case 1: //Special case for testing stream 1
			config->SetCodec("MJPEG");
			config->SetResolution("640x400");
			config->SetFrameRate(30);
			config->SetEnabled(false);
			break;
		default:
			config->SetCodec("H264");
			config->SetResolution("640x400");
			config->SetFrameRate(30);
			config->SetEnabled(true);
			break;
	}
}

bool Example::VideoInputPipeline(unsigned int input, VideoInputConfig *config, std::string *str)
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
		pipe << "videotestsrc horizontal-speed=5 is-live=true ! ";
		pipe << "capsfilter caps=capsfilter caps=\"video/x-raw, framerate=" << config->GetFrameRate() << "/1";
		pipe << ", width=" << width << " , height=" << height << "\" ! ";
		pipe << "x264enc key-int-max=" << config->GetFrameRate() << " threads=1 ! ";
		pipe << "video/x-h264, stream-format=avc, alignment=au ! ";
		pipe << "internalsink streamname=video" << input;

	} else if (config->GetCodec() == "MJPEG")
	{
		pipe << "videotestsrc horizontal-speed=5 is-live=true ! ";
		pipe << "capsfilter caps=capsfilter caps=\"video/x-raw, framerate=" << config->GetFrameRate() << "/1";
		pipe << ", width=" << width << " , height=" << height << "\" ! ";
		pipe << "jpegenc ! ";
		pipe << "internalsink streamname=video" << input;

	}
	else
	{
		LogError("Example::VideoInputEnable - Unknown Codec");
		return false;
	}

	*str = pipe.str();
	return true;
}


unsigned int Example::VideoOutputCount()
{
	return 1;
}

void Example::VideoOutputSupportedInfo(unsigned int output, VideoOutputSupported *info)
{
	info->Clear(); //Reset to defaults
}

unsigned int Example::GPIOOutputCount()
{
	LogDebug("Example::GPIOOutputCount()");
	return GPIO_OUTPUTS;
}

void Example::GPIOOutputSetState(unsigned int output, bool enabled)
{
	LogDebug("Example::GPIOOutputSetState(%u, %s)", output, enabled ? "On" : "off");
	if (m_gpio_outputs[output] == enabled)
	{
		return;
	}
	else
	{
		LogNotice("GPIO Output State Changed to %s", enabled ? "On" : "Off");
		m_gpio_outputs[output] = enabled;
	}
}

bool Example::GPIOOutputGetState(unsigned int output)
{
	LogDebug("Example::GPIOOutputGetState(%u)", output);
	return m_gpio_outputs[output];
}


