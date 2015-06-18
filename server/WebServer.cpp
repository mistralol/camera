
#include <Camera.h>

WebServer::WebServer()
{
	m_enabled = true;
	m_port = 8080;
}

WebServer::~WebServer()
{
	Stop();
}

bool WebServer::ConfigLoad(Json::Value &json)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	
	
	if (json.isMember("enabled") && json["enabled"].isBool())
		SetEnabled(json["enabled"].asBool());
	
	if (json.isMember("port") && json["port"].isNumeric())
	{
		if (SetPort(json["port"].asInt()) < 0)
		{
			LogError("WebServer::ConfigLoad - Failed to set port");
			return false;
		}
	}
	
	return true;
}

bool WebServer::ConfigSave(Json::Value &json)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("WebServer::ConfigSave");

	json["enabled"] = GetEnabled();
	json["port"] = GetPort();
	return true;
}
		
void WebServer::Start()
{
	ScopedLock lock = ScopedLock(&m_mutex);
	if (m_enabled == false)
		return;
	LogInfo("WebServer Starting On Port %d", m_port);
	
}

void WebServer::Stop()
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogInfo("WebServer Stopping");
}

void WebServer::Restart()
{
	ScopedLock lock = ScopedLock(&m_mutex);
	Stop();
	Start();
}

void WebServer::SetEnabled(bool enabled)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	m_enabled = enabled;
	Restart();
}

bool WebServer::GetEnabled()
{
	ScopedLock lock = ScopedLock(&m_mutex);
	return m_enabled;
}
		
int WebServer::SetPort(int port)
{
	if (port <= 0 || port >= 65535)
	{
		LogError("WebServer::SetPort(%d) - Invalid Port", port);
		return -ERANGE;
	}
	
	ScopedLock lock = ScopedLock(&m_mutex);
	m_port = port;
	Restart();
	return port;
}

int WebServer::GetPort()
{
	ScopedLock lock = ScopedLock(&m_mutex);
	return m_port;
}

