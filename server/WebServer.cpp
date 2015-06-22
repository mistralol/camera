
#include <Camera.h>

WebServer::WebServer()
{
	m_enabled = true;
	m_port = 8080;
	m_pid = -1;
}

WebServer::~WebServer()
{
	Stop();
}

bool WebServer::ConfigLoad(Json::Value &json)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	m_props.clear();
	
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
	
	if (json.isMember("props"))
	{
		Json::Value &props = json["props"];
		for(unsigned int idx = 0; idx < props.size(); idx++)
		{
			std::string key = props[idx].asString();
			std::string value = props[key].asString();
			m_props[key] = value;
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
	
	for(std::map<std::string, std::string>::iterator it = m_props.begin(); it != m_props.end(); it++)
	{
		json["props"][it->first] = it->second;
	}

	
	return true;
}
		
void WebServer::Start()
{
	ScopedLock lock = ScopedLock(&m_mutex);
	if (m_enabled == false)
		return;
	LogInfo("WebServer Starting On Port %d", m_port);

	if (m_pid >= 0)
	{
		LogCritical("WebServer::Start() - WebServer already started? aborting....");
		abort(); //Invalid state tried to start server twice!
	}
	
	//Build string for port for argument passing
	std::stringstream ss;
	ss << m_port;
	std::string sport = ss.str();
	
	pid_t pid = fork();
	if (pid < 0)
	{
		LogError("WebServer::Start() - fork() failed '%s'", strerror(errno));
		return;
	}

	if (pid == 0)
	{
		//Child process. Start xsp4 to act as a web server
		//FIXME: Hardcoded values
		
		//Close All fd's
		for(int i=3;i<1024;i++)
			close(i);
		
		//Restore all signals or mono freak's out!
		sigset_t all;
		if (sigfillset(&all) < 0)
			abort();

		if (sigprocmask(SIG_UNBLOCK, &all, NULL) < 0)
			abort();
			

		
		if (execl("/usr/bin/xsp4", "/usr/bin/xsp4", "--nonstop", "--root", "/home/james/src/camera/WebUI", "--port", sport.c_str(), NULL) < 0)
		{
			printf("execl failed: %s\n", strerror(errno));
			abort();
		}
	}

	//Parent Prcoess - Start monitoring thread
	LogInfo("WebServer Process Has pid %d", pid);
	m_pid = pid;
	Thread::Start();
	return;
}

void WebServer::Stop()
{
	ScopedLock lock = ScopedLock(&m_mutex);
	if (m_pid < 0)
		return; //No server running
	LogInfo("WebServer Stopping");
	if (kill(m_pid, 9) < 0)
	{
		LogError("WebServer::Stop() - Kill failed '%s'", strerror(errno));
	}
	Thread::Stop(); //Accept that m_pid is set to -1
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
	return port;
}

int WebServer::GetPort()
{
	ScopedLock lock = ScopedLock(&m_mutex);
	return m_port;
}

std::string WebServer::GetProperty(const std::string key)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	std::map<std::string, std::string>::iterator it = m_props.find(key);
	if (it == m_props.end())
		return "";
	return it->second;
}

std::string WebServer::GetProperty(const std::string key, const std::string def)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	std::map<std::string, std::string>::iterator it = m_props.find(key);
	if (it == m_props.end())
		return def;
	return it->second;
}

void WebServer::SetProperty(const std::string key, const std::string value)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	m_props[key] = value;
}

void WebServer::Run()
{
	bool running = true;
	int status = 0;

	while(running)
	{
		pid_t ret = waitpid(m_pid, &status, 0);
		if (ret < 0)
		{
			switch(errno)
			{
				case EINTR:
					return;
					break;
				case ECHILD:
					LogError("WebServer::Run() - waitpid failed '%s'", strerror(errno));
					return;
					break;
				case EINVAL:
					abort();
					break;
			}
			LogError("WebServer::Run() - waitpid failed '%s'", strerror(errno));
			continue;
		}
		m_pid = -1;
		LogWarning("WebServer::Run() - WebServer exited");
		return;
	}
}


