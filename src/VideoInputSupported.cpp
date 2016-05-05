
#include <stdlib.h>
#include <string>
#include <sstream>
#include <map>
#include <list>

#include <json/json.h>

#include <VideoInputSupported.h>

VideoInputSupported::VideoInputSupported() :
	hflip(0), vflip(0)
{

}

VideoInputSupported::~VideoInputSupported()
{

}

std::vector<std::string> VideoInputSupported::GetCodecs()
{
	std::vector<std::string> lst;
	std::map<std::string, struct CodecInfo>::iterator it = m_codecs.begin();
	while(it != m_codecs.end())
	{
		lst.push_back(it->first);
		it++;
	}
	return lst;
}

std::vector<std::string> VideoInputSupported::GetCodecResolutions(const std::string &codec)
{
	std::vector<std::string> lst;
	std::map<std::string, struct CodecInfo>::iterator it = m_codecs.find(codec);
	if (it == m_codecs.end())
		return lst;

	std::map<std::string, std::vector<int> >::iterator resit = it->second.m_res.begin();
	while(resit != it->second.m_res.end())
	{
		lst.push_back(resit->first);
		resit++;
	}
	return lst;
}

std::vector<int> VideoInputSupported::GetCodecFrameRates(const std::string &codec, const std::string &res)
{
	std::vector<int> lst;
	std::map<std::string, struct CodecInfo>::iterator it = m_codecs.find(codec);
	if (it == m_codecs.end())
		return lst;

	std::map<std::string, std::vector<int> >::iterator resit = it->second.m_res.find(res);
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
	hflip = 0;
	vflip = 0;
}

std::vector<std::string> VideoInputSupported::ToStrV()
{
	std::vector<std::string> lst;
	std::map<std::string, struct CodecInfo>::iterator it = m_codecs.begin();
	while(it != m_codecs.end())
	{
		std::map<std::string, std::vector<int> >::iterator resit = it->second.m_res.begin();
		while(resit != it->second.m_res.end())
		{
			std::vector<int>::iterator fpsit = resit->second.begin();
			std::stringstream ss;
			while(fpsit != resit->second.end())
			{
				ss << *fpsit;
				fpsit++;
				if (fpsit != resit->second.end())
					ss << ", ";
			}
			std::stringstream line;
			line << "Codec: " << it->first << " Res: " << resit->first << " FPS: " << ss.str();
			lst.push_back(line.str());
			resit++;
		}
		it++;
	}
	return lst;
}

std::string VideoInputSupported::Encode()
{
	Json::Value json;

	json["codecs"] = Json::arrayValue;
	std::map<std::string, struct CodecInfo>::iterator it = m_codecs.begin();
	while(it != m_codecs.end())
	{
		std::string codec = it->first;
		struct CodecInfo *info = &it->second;
		
		json["codecs"].append(codec);
				
		Json::Value res;
		std::map<std::string, std::vector<int> >::iterator it2 = info->m_res.begin();
		while(it2 != info->m_res.end())
		{
			std::string resolution = it2->first;
			std::vector<int> *framerates = &it2->second;

			Json::Value rates = Json::arrayValue;
			std::vector<int>::iterator it3 = framerates->begin();
			while(it3 != framerates->end())
			{
				rates.append(*it3);
				it3++;
			}
			it2++;
			res[resolution] = rates;
		}
		json[codec]["resolution"] = res;
		it++;
	}
	
	json["hflip"] = hflip;
	json["vflip"] = vflip;
	
	std::stringstream ss;
	Json::StyledWriter styledWriter;
	ss << styledWriter.write(json);
	return ss.str();
}

bool VideoInputSupported::Decode(const std::string str)
{
	Json::Value root;
	Json::Reader reader;

	Clear();

	if (reader.parse(str, root ) == false)
		return false;

	hflip = root["hflip"].asInt();
	vflip = root["vflip"].asInt();

	if (root.isMember("codecs") == false || root["codecs"].isArray() == false)
		return false;
	
	try {	
		for(int i =0;i<root["codecs"].size();i++)
		{
			std::string codec = root["codecs"][i].asString();
			if (root.isMember(codec) == false)
				return false;
	
			Json::Value jcodec = root[codec];
			if (jcodec.isMember("resolution") == false)
				return false;

			std::vector<std::string> lst = jcodec["resolution"].getMemberNames();
			for(std::vector<std::string>::iterator it = lst.begin(); it != lst.end(); it++)
			{
				std::string res = *it;
			
				Json::Value arr = jcodec["resolution"][res];
				if (arr.isArray() == false)
					return false;
				for(int j=0;j<arr.size();j++)
				{
					if (arr[j].isNumeric() == false)
						return false;
					AddCodec(codec, res, arr[j].asInt());
				}
			}
		
		}
	} catch(...)
	{
		return false;
	}
			
	return true;	
}

