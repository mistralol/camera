
#include <cameractl.h>

static struct Operations Ops[] = {
	{"help", WebServer::Help, "Shows this help"},
	{"enabled", WebServer::Enabled, "enable / disable webserver"},
	{"restart", WebServer::Restart, "restart webserver"},
	{"port", WebServer::Port, "get / set port for webserver"},
	{NULL, NULL }
};

int WebServer::Enabled(struct Data *data)
{
	int enabled = 0;
	if (data->args.size() == 0)
	{
		int ret = data->cli->WebServerGetEnabled(&enabled);
		if (ret < 0)
			return ret;
		return enabled;
	}

	std::string str = data->args.front();
	if (str == "1")
		enabled = true;
	return data->cli->WebServerSetEnabled(enabled);
}

int WebServer::Restart(struct Data *data)
{
	return data->cli->WebServerRestart();
}

int WebServer::Port(struct Data *data)
{
	int port = 0;
	if (data->args.size() == 0)
	{
		int ret = data->cli->WebServerGetEnabled(&port);
		if (ret < 0)
			return ret;
		return port;
	}

	std::string str = data->args.front();
	port = atoi(str.c_str());
	int ret = data->cli->WebServerSetEnabled(port);
	if (ret < 0)
		return ret;
	return port;
}

int WebServer::Help(struct Data *data)
{
	return DumpHelp(Ops);
}

int WebServer::Process(struct Data *data)
{
	data->ops = Ops;
	return ::Process(data);
}


