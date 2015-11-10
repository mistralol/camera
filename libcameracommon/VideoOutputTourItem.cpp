
#include <string>
#include <json/json.h>

#include <VideoOutputTourItem.h>

VideoOutputTourItem::VideoOutputTourItem()
{

}

VideoOutputTourItem::VideoOutputTourItem(const std::string src, int delay)
{
	m_source = src;
	m_delay = delay;
}

VideoOutputTourItem::~VideoOutputTourItem()
{

}

std::string VideoOutputTourItem::GetSource()
{
	return m_source;
}

void VideoOutputTourItem::SetSource(const std::string source)
{
	m_source = source;
}
		
int VideoOutputTourItem::GetDelay()
{
	return m_delay;
}

void VideoOutputTourItem::SetDelay(int seconds)
{
	m_delay = seconds;
}

bool VideoOutputTourItem::ConfigSave(Json::Value &json)
{
	json["source"] = m_source;
	json["delay"] = m_delay;
	return true;
}

bool VideoOutputTourItem::ConfigLoad(Json::Value &json)
{
	if (json.isMember("source") && json["source"].isString())
		m_source = json["source"].asString();

	if (json.isMember("delay") && json["delay"].isNumeric())
		m_delay = json["delay"].asInt();
		
	return true;
}

