
#include <Camera.h>

#include <PI2Camera.h>

extern "C" PlatformBase *Create()
{
	return new PI2Camera();
}

PI2Camera::PI2Camera()
{

}

PI2Camera::~PI2Camera()
{

}

bool PI2Camera::Init()
{
	LogDebug("PI2Camera::Init");

	return true;
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
	info->AddCodec("H264", "1280x1080", 1, 30);

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

bool PI2Camera::VideoInputConfigure(unsigned int input, const VideoInputConfig *config)
{
	LogDebug("PI2Camera::VideoInputConfigure(%u, \"%s\")", input, config->ToStr().c_str());
	m_videoinputconfig[input] = *config; //Take a copy
	return true;
}

bool PI2Camera::VideoInputEnable(unsigned int input)
{
	LogDebug("PI2Camera::VideoInputEnable(%u)", input);
	if (m_videoinputconfig.find(input) == m_videoinputconfig.end())
		abort(); //Bug because VideoInputEnable is called before VideoInputConfigure
	if (m_videoinputpipelines.find(input) != m_videoinputpipelines.end())
		abort(); //WTF? We already have a pipeline which means the input is enabled

	std::stringstream pipename;
	pipename << "PI2Camera Encoder " << input;

	std::stringstream pipe;
	std::string res = m_videoinputconfig[input].GetResolution();
	std::string width = "";
	std::string height = "";
	if (String::SplitOne(&res, &width, &height, "x") == false)
	{
		LogError("PI2Camera::VideoInputEnable - Failed to parse: %s", res.c_str());
		return false;
	}
	if (m_videoinputconfig[input].GetCodec() == "H264")
	{
		pipe << "rpicamsrc preview=false keyframe-interval=" << m_videoinputconfig[input].GetFrameRate();
		pipe << " ! capsfilter caps=capsfilter caps=\"video/x-raw, framerate=" << m_videoinputconfig[input].GetFrameRate() << "/1";
		pipe << ", width=" << width << " , height=" << height << "\"";
		pipe << " ! h264parse";
		pipe << " ! video/x-h264, stream-format=avc, alignment=au";
		pipe << " ! internalsink streamname=video" << input;
		
		LogDebug("PI2Camera::VideoInputEnable - Pipeline %s", pipe.str().c_str());
		LogInfo("PI2Camera::VideoInputEnable - Starting Input %u", input);
		PipelineBasic *pipeline = new PipelineBasic(pipename.str(), pipe.str());
		m_videoinputpipelines[input] = pipeline;
		m_videoinputpipelines[input]->Start();
		return true;
	}
	else
	{
		LogError("PI2Camera::VideoInputEnable - Unknown Codec");
	}

	return false;
}

bool PI2Camera::VideoInputDisable(unsigned int input)
{
	LogDebug("PI2Camera::VideoInputDisable(%u)", input);
	std::map<unsigned int, PipelineBasic *>::iterator it = m_videoinputpipelines.find(input);
	if (it == m_videoinputpipelines.end())
	{
		LogError("PI2Camera::VideoInputDisable Failed because there is no pipeline");
		return false;
	}
	PipelineBasic *pipeline = it->second;
	m_videoinputpipelines.erase(it);
	LogInfo("PI2Camera::VideoInputDisable - Stopping Input %u", input);
	pipeline->Stop();
	delete pipeline;
	return true;
}


