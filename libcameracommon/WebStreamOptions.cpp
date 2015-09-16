
#include <string>
#include <sstream>
#include <json/json.h>

#include <stdio.h>

#include <WebStreamType.h>
#include <WebStreamOptions.h>

WebStreamOptions::WebStreamOptions() :
	vinput(0), type(Unknown), localonly(true), timeout(60)
{

}

bool WebStreamOptions::Encode(std::string &str)
{
	Json::Value json;
	
	json["vinput"] = vinput;
	json["type"] = type;
	json["localonly"] = localonly;
	json["timeout"] = timeout;
	
	std::stringstream ss;
	Json::StyledWriter styledWriter;
	ss << styledWriter.write(json);
	str = ss.str();
	return true;
}

bool WebStreamOptions::Decode(const std::string &str)
{
	Json::Value root;
	Json::Reader reader;

        if (reader.parse(str, root ) == false)
		return false;
		
	vinput = root["vinput"].asInt();
	type = (WebStreamType) root["type"].asInt();
	localonly = root["localonly"].asBool();
	timeout = root["timeout"].asInt();
		
	return true;
}

