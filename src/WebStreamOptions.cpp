
#include <Camera.h>

WebStreamOptions::WebStreamOptions() :
	vinput(0),
	type("Unknown"),
	localonly(true),
	timeout(60)
{

}

bool WebStreamOptions::Encode(Json::Value &json)
{
	json["vinput"] = vinput;
	json["type"] = type;
	json["localonly"] = localonly;
	json["timeout"] = timeout;

	return true;
}

bool WebStreamOptions::Decode(const Json::Value &json)
{
	if (json.isMember("vinput") && json["vinput"].isInt())
	{
		vinput = json["vinput"].asInt();
	}
	else
	{
		LogError("WebStreamOptions::Decode - Missing Parameter vinput");
		throw("Missing Parameter vinput");
	}
	
	if (json.isMember("type") && json["type"].isString())
	{
		type = json["type"].asString();	
	}
	else
	{
		LogError("WebStreamOptions::Decode - Missing Parameter type");
		throw("Missing Parameter type");
	}

	if (json.isMember("localonly"))
	{
		if (json["localonly"].isBool())
		{
			localonly = json["localonly"].asBool();
		}
		else
		{
			LogWarning("WebStreamOptions::Decode - Ignoring localonly option as its not a bool!");
		}
	}

	if (json.isMember("timeout"))
	{
		if (json["timeout"].isInt())
		{
			timeout = json["timeout"].asInt();		
		}
		else
		{
			LogWarning("WebStreamOptions::Decode - Ignoring timeout option as its not an int!");
		}
	}

	
	return true;
}

