
#include <Camera.h>


VideoStreamConfig::VideoStreamConfig()
{
	m_framerate = 0;
	m_codec = "";
	m_resolution = "";
}

VideoStreamConfig::~VideoStreamConfig()
{

}

int VideoStreamConfig::GetFrameRate()
{
	return m_framerate;
}

void VideoStreamConfig::SetFrameRate(int fps)
{
	m_framerate = fps;
}

std::string VideoStreamConfig::GetCodec()
{
	return m_codec;
}

void VideoStreamConfig::SetCodec(const std::string &str)
{
	m_codec = str;
}

std::string VideoStreamConfig::GetResolution()
{
	return m_resolution;
}

void VideoStreamConfig::SetResolutions(const std::string &str)
{
	m_resolution = str;
}

