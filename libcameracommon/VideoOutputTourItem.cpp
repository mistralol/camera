
#include <string>

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


