
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

bool VideoStreamConfig::ConfigSave(Json::Value &json)
{
	json["framerate"] = m_framerate;
	json["codec"] = m_codec;
	json["resolution"] = m_resolution;
	json["enabled"] = m_enabled;
	return true;
}

bool VideoStreamConfig::ConfigLoad(Json::Value &json)
{
	if (json.isMember("framerate") && json["framerate"].isNumeric())
		m_framerate = json["framerate"].asInt();

	if (json.isMember("codec") && json["codec"].isString())
		m_codec = json["codec"].asString();

	if (json.isMember("resolution") && json["resolution"].isString())
		m_resolution = json["resolution"].asString();

	if (json.isMember("enabled") && json["enabled"].isBool())
		m_enabled = json["enabled"].asBool();

	return true;
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

