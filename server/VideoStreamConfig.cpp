
#include <Camera.h>


VideoStreamConfig::VideoStreamConfig()
{
	m_framerate = 0;
	m_codec = "";
	m_resolution = "";
	m_enabled = false;
}

VideoStreamConfig::~VideoStreamConfig()
{

}

std::string VideoStreamConfig::ToString() const
{
	std::stringstream ss;
	ss << m_codec << "/" << m_resolution << " @ " << m_framerate;
	return ss.str();
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

void VideoStreamConfig::SetResolution(const std::string &str)
{
	m_resolution = str;
}

bool VideoStreamConfig::GetEnabled()
{
	return m_enabled;
}

void VideoStreamConfig::SetEnabled(bool enabled)
{
	m_enabled = enabled;
}

