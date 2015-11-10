
#include <list>
#include <string>

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


