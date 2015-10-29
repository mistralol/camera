
#include <cameractl.h>

static struct Operations Ops[] = {
	{"help", WebServer::Help, "Shows this help"},
	{"enabled", WebServer::Enabled, "enable / disable webserver"},
	{"restart", WebServer::Restart, "restart webserver"},
	{"port", WebServer::Port, "get / set port for webserver"},
	{NULL, NULL }
};

void WebServer::Enabled(struct Data *data)
{
	int enabled = 0;
	if (data->args.size() == 0)
	{
		int ret = data->cli->WebServerGetEnabled();
		printf("Enabled: %d", ret);
		return;
	}

	std::string str = data->args.front();
	if (str == "1")
		enabled = true;
	data->cli->WebServerSetEnabled(enabled);
}

void WebServer::Restart(struct Data *data)
{
	data->cli->WebServerRestart();
}

void WebServer::Port(struct Data *data)
{
	int port = 0;
	if (data->args.size() == 0)
	{
		int ret = data->cli->WebServerGetPort();
		printf("Port: %d", ret);
		return;
	}

	std::string str = data->args.front();
	port = atoi(str.c_str());
	data->cli->WebServerSetPort(port);
}

void WebServer::Help(struct Data *data)
{
	DumpHelp(Ops);
}

void WebServer::Process(struct Data *data)
{
	data->ops = Ops;
	::Process(data);
}


