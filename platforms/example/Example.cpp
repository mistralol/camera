
#include <Camera.h>

#include <Example.h>

extern "C" PlatformBase *Create()
{
	return new Example();
}

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
	return true;
}

bool Example::ConfigSave(Json::Value &json)
{
	return true;
}

unsigned int Example::VideoInputCount()
{
	LogDebug("Example::VideoInputCount");
	return 1;
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
	info->AddCodec("H264", "1280x1080", 1, 30);
	return true;
}

void Example::VideoInputDefaultConfig(unsigned int input, VideoInputConfig *config)
{
	config->SetCodec("H264");
	config->SetResolution("640x400");
	config->SetFrameRate(30);
	config->SetEnabled(true);
}

bool Example::VideoInputConfigure(unsigned int input, const VideoInputConfig *config)
{
	LogDebug("Example::VideoInputConfigure(%u, \"%s\")", input, config->ToString().c_str());
	m_videoinputconfig[input] = *config; //Take a copy
	return true;
}

bool Example::VideoInputEnable(unsigned int input)
{
	LogDebug("Example::VideoInputEnable(%u)", input);
	if (m_videoinputconfig.find(input) == m_videoinputconfig.end())
		abort(); //Bug because VideoInputEnable is called before VideoInputConfigure
	if (m_videoinputpipelines.find(input) != m_videoinputpipelines.end())
		abort(); //WTF? We already have a pipeline which means the input is enabled

	std::stringstream pipe;
	std::string res = m_videoinputconfig[input].GetResolution();
	std::string width = "";
	std::string height = "";
	if (String::SplitOne(&res, &width, &height, "x") == false)
	{
		LogError("Example::VideoInputEnable - Failed to parse: %s", res.c_str());
		return false;
	}
	if (m_videoinputconfig[input].GetCodec() == "H264")
	{
		pipe << "videotestsrc horizontal-speed=5 is-live=true ! " <<
			"capsfilter caps=capsfilter caps=\"video/x-raw, framerate=" << m_videoinputconfig[input].GetFrameRate() << "/1, width=" << width << " , height=" << height << "\" ! " <<
			"x264enc key-int-max=30 byte-stream=true ! " <<
			"internalsink streamname=video" << input;
		LogDebug("Example::VideoInputEnable - Pipeline %s", pipe.str().c_str());
		LogInfo("Example::VideoInputEnable - Starting Input %u", input);
		PipelineBasic *pipeline = new PipelineBasic(pipe.str());
		m_videoinputpipelines[input] = pipeline;
		m_videoinputpipelines[input]->Start();
		return true;
	}
	else
	{
		LogError("Example::VideoInputEnable - Unknown Codec");
	}

	return false;
}

bool Example::VideoInputDisable(unsigned int input)
{
	LogDebug("Example::VideoInputDisable(%u)", input);
	std::map<unsigned int, PipelineBasic *>::iterator it = m_videoinputpipelines.find(input);
	if (it == m_videoinputpipelines.end())
	{
		LogError("Example::VideoInputDisable Failed because there is no pipeline");
		return false;
	}
	PipelineBasic *pipeline = it->second;
	m_videoinputpipelines.erase(it);
	pipeline->Stop();
	delete pipeline;
	return true;
}


