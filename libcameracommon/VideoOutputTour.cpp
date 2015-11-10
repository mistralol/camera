
#include <list>
#include <string>

#include <sstream>
#include <json/json.h>

#include <VideoOutputTourItem.h>
#include <VideoOutputTour.h>

VideoOutputTour::VideoOutputTour()
{

}

VideoOutputTour::VideoOutputTour(const std::string name)
{
	m_name = name;
}

VideoOutputTour::~VideoOutputTour()
{

}
		
std::string VideoOutputTour::GetName()
{
	return m_name;
}

void VideoOutputTour::SetName(const std::string name)
{
	m_name = name;
}
		
void VideoOutputTour::AddSource(const VideoOutputTourItem &item)
{
	m_items.push_back(item);
}

void VideoOutputTour::AddSource(const std::string source, int delay)
{
	VideoOutputTourItem item = VideoOutputTourItem(source, delay);
	AddSource(item);
}

void VideoOutputTour::Clear()
{
	m_items.clear();
}

bool VideoOutputTour::ConfigSave(Json::Value &json)
{
	json["name"] = m_name;
	
	for(std::list<VideoOutputTourItem>::iterator it=m_items.begin();it!=m_items.end();it++)
	{
		Json::Value item;
		it->ConfigSave(item);
		json["items"].append(item);
	}

	return true;
}

bool VideoOutputTour::ConfigLoad(Json::Value &json)
{
	if (json.isMember("name") && json["name"].isString())
		m_name = json["name"].asString();

	if (json.isMember("items") == false || json["items"].isArray() == false)
		return false;

	m_items.clear();
	for(int i =0;i<json["items"].size();i++)
	{
		VideoOutputTourItem item;
		if (item.ConfigLoad(json["items"][i]) == false)
			return false;
		m_items.push_back(item);
	}

	return true;
}

std::string VideoOutputTour::Encode()
{
	Json::Value json;
	ConfigSave(json);
	std::stringstream ss;
	Json::StyledWriter styledWriter;
	ss << styledWriter.write(json);
	return ss.str();
}

bool VideoOutputTour::Decode(const std::string str)
{
	Json::Value root;
	Json::Reader reader;

	if (reader.parse(str, root ) == false)
		return false;
	
	return ConfigLoad(root);
}


