
#include <Camera.h>

bool Debug::m_enabled = false;

bool Debug::ConfigLoad(Json::Value &json)
{
	LogDebug("Debug::ConfigLoad");
	if (json.isMember("enabled") && json["enabled"].isBool())
		SetEnabled(json["enabled"].asBool());

	return true;
}

bool Debug::ConfigSave(Json::Value &json)
{
	LogDebug("Debug::ConfigSave");
	json["enabled"] = GetEnabled();
	
	return true;
}

void Debug::SetEnabled(bool enabled)
{
	LogInfo("Debug set to %s", enabled ? "On" : "Off");

	if (enabled)
	{
		LogManager::SetLevel(LOGGER_DEBUG);
	}
	else
	{
		LogManager::SetLevel(LOGGER_INFO);
	}
	
	m_enabled = enabled;
}

bool Debug::GetEnabled()
{
	return m_enabled;
}

bool IsDebug()
{
	return Debug::GetEnabled();
}

