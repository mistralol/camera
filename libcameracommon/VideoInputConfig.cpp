
#include <string>
#include <sstream>
#include <json/json.h>

#include <VideoInputConfig.h>


VideoInputConfig::VideoInputConfig()
{
	m_framerate = 0;
	m_codec = "";
	m_resolution = "";
	m_enabled = false;
}

VideoInputConfig::~VideoInputConfig()
{

}

std::string VideoInputConfig::ToStr() const
{
	std::stringstream ss;
	ss << m_codec << "/" << m_resolution << " @ " << m_framerate;
	return ss.str();
}

bool VideoInputConfig::ConfigSave(Json::Value &json)
{
	json["framerate"] = m_framerate;
	json["codec"] = m_codec;
	json["resolution"] = m_resolution;
	json["enabled"] = m_enabled;
	return true;
}

bool VideoInputConfig::ConfigLoad(Json::Value &json)
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

int VideoInputConfig::GetFrameRate()
{
	return m_framerate;
}

void VideoInputConfig::SetFrameRate(int fps)
{
	m_framerate = fps;
}

std::string VideoInputConfig::GetCodec()
{
	return m_codec;
}

void VideoInputConfig::SetCodec(const std::string &str)
{
	m_codec = str;
}

std::string VideoInputConfig::GetResolution()
{
	return m_resolution;
}

void VideoInputConfig::SetResolution(const std::string &str)
{
	m_resolution = str;
}

bool VideoInputConfig::GetEnabled()
{
	return m_enabled;
}

void VideoInputConfig::SetEnabled(bool enabled)
{
	m_enabled = enabled;
}

std::string VideoInputConfig::Encode()
{
	Json::Value json;
	
	json["framerate"] = m_framerate;
	json["codec"] = m_codec;
	json["resolution"] = m_resolution;
	json["enabled"] = m_enabled;
	
	std::stringstream ss;
	Json::StyledWriter styledWriter;
	ss << styledWriter.write(json);
	return ss.str();
}

bool VideoInputConfig::Decode(const std::string str)
{
	Json::Value root;
	Json::Reader reader;

	if (reader.parse(str, root ) == false)
		return false;
		
	try {
		m_framerate = root["recordingSchedule"].asInt();
		m_codec = root["codec"].asString();
		m_resolution = root["resolution"].asString();
		m_enabled = root["enabled"].asBool();
	} catch(...)
	{
		return false;
	}
			
	return true;		
}

