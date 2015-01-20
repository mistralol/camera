
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
	return true;
}

bool Example::ConfigSave(Json::Value &json)
{
	return true;
}

unsigned int Example::VideoStreamCount()
{
	LogDebug("Example::GetVideoNumStreams");
	return 1;
}

bool Example::VideoStreamSupportedInfo(unsigned int stream, VideoStreamSupported *info)
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

void Example::VideoStreamDefaultConfig(unsigned int stream, VideoStreamConfig *config)
{
	config->SetCodec("H264");
	config->SetResolution("640x400");
	config->SetFrameRate(30);
	config->SetEnabled(true);
}

bool Example::VideoStreamConfigure(unsigned int stream, const VideoStreamConfig *config)
{
	LogDebug("Example::VideoStreamConfigure(%u, \"%s\")", stream, config->ToString().c_str());
	m_videoconfig[stream] = *config; //Take a copy
	return true;
}

bool Example::VideoStreamEnable(unsigned int stream)
{
	LogDebug("Example::VideoStreamEnable(%u)", stream);
	if (m_videoconfig.find(stream) == m_videoconfig.end())
		abort(); //Bug because VideoStreamEnable is called before VideoStreamConfigure
	if (m_videopipelines.find(stream) != m_videopipelines.end())
		abort(); //WTF? We already have a pipeline which means the stream is enabled

	std::stringstream pipe;
	std::string res = m_videoconfig[stream].GetResolution();
	std::string width = "";
	std::string height = "";
	if (String::SplitOne(&res, &width, &height, "x") == false)
	{
		LogError("Example::VideoStreamEnable - Failed to parse: %s", res.c_str());
		return false;
	}
	if (m_videoconfig[stream].GetCodec() == "H264")
	{
		pipe << "videotestsrc horizontal-speed=5 is-live=true ! " <<
			"capsfilter caps=capsfilter caps=\"video/x-raw, framerate=" << m_videoconfig[stream].GetFrameRate() << "/1, width=" << width << " , height=" << height << "\" ! " <<
			"x264enc key-int-max=30 intra-refresh=true ! " <<
			"fakesink";
		LogDebug("Example::VideoStreamEnable - Pipeline %s", pipe.str().c_str());
		LogInfo("Example::VideoStreamEnable - Starting Stream %u", stream);
		PipelineBasic *pipeline = new PipelineBasic(pipe.str());
		m_videopipelines[stream] = pipeline;
		m_videopipelines[stream]->Start();
		return true;
	}
	else
	{
		LogError("Example::VideoStreamEnable - Unknown Codec");
	}

	return false;
}

bool Example::VideoStreamDisable(unsigned int stream)
{
	LogDebug("Example::VideoStreamDisable(%u)", stream);
	std::map<unsigned int, PipelineBasic *>::iterator it = m_videopipelines.find(stream);
	if (it == m_videopipelines.end())
	{
		LogError("Example::VideoStreamDisable Failed because there is no pipeline");
		return false;
	}
	PipelineBasic *pipeline = it->second;
	m_videopipelines.erase(it);
	pipeline->Stop();
	delete pipeline;
	return true;
}


