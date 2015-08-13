
#include <stdlib.h>
#include <string>
#include <sstream>
#include <map>
#include <list>

#include <liblogger.h>
using namespace liblogger;


#include <VideoInputSupported.h>

VideoInputSupported::VideoInputSupported()
{

}

VideoInputSupported::~VideoInputSupported()
{

}

std::list<std::string> VideoInputSupported::GetCodecs()
{
	std::list<std::string> lst;
	std::map<std::string, struct CodecInfo>::iterator it = m_codecs.begin();
	while(it != m_codecs.end())
	{
		lst.push_back(it->first);
		it++;
	}
	return lst;
}

std::list<std::string> VideoInputSupported::GetCodecResolutions(const std::string &codec)
{
	std::list<std::string> lst;
	std::map<std::string, struct CodecInfo>::iterator it = m_codecs.find(codec);
	if (it == m_codecs.end())
		return lst;

	std::map<std::string, std::list<int> >::iterator resit = it->second.m_res.begin();
	while(resit != it->second.m_res.end())
	{
		lst.push_back(resit->first);
		resit++;
	}
	return lst;
}

std::list<int> VideoInputSupported::GetCodecFrameRates(const std::string &codec, std::string &res)
{
	std::list<int> lst;
	std::map<std::string, struct CodecInfo>::iterator it = m_codecs.find(codec);
	if (it == m_codecs.end())
		return lst;

	std::map<std::string, std::list<int> >::iterator resit = it->second.m_res.find(res);
	if (resit == it->second.m_res.end())
		return lst;

	return resit->second;
}

void VideoInputSupported::AddCodec(const std::string &codec, const std::string &res, int fps)
{
	m_codecs[codec].m_res[res].push_back(fps);
}

void VideoInputSupported::AddCodec(const std::string &codec, const std::string &res, int lowfps, int upperfps)
{
	if (lowfps > upperfps)
		abort();
	for(int i=lowfps;i<=upperfps;i++)
		AddCodec(codec, res, i);
}

void VideoInputSupported::Clear()
{
	m_codecs.clear();
}

void VideoInputSupported::LogDump()
{
	std::map<std::string, struct CodecInfo>::iterator it = m_codecs.begin();
	while(it != m_codecs.end())
	{
		std::map<std::string, std::list<int> >::iterator resit = it->second.m_res.begin();
		while(resit != it->second.m_res.end())
		{
			std::list<int>::iterator fpsit = resit->second.begin();
			std::stringstream ss;
			while(fpsit != resit->second.end())
			{
				ss << *fpsit;
				fpsit++;
				if (fpsit != resit->second.end())
					ss << ", ";
			}
			LogInfo("Codec: %s Res: %s FPS: %s", it->first.c_str(), resit->first.c_str(), ss.str().c_str());
			resit++;
		}
		it++;
	}
}


