
#include <cameractl.h>

static struct Operations Ops[] = {
	{"help", Rtsp::Help, "Shows this help"},
	{"max-backlog", Rtsp::MaxBacklog, "Get / Set max backlog"},
	{"max-clients", Rtsp::MaxClients, "Get / Set max client connections"},
	{"port", Rtsp::Port, "Get / Set RTSP port"},
	{"kickall", Rtsp::KickAll, "Kick all clients"},
	{"count", Rtsp::Count, "Count of connected clients"},
	{NULL, NULL }
};

void Rtsp::MaxBacklog(struct Data *data)
{
	int max = 0;
	if (data->args.size() == 0)
	{
		int ret = data->cli->RTSPGetMaxBacklog();
		printf("value: %d\n", ret);
		return;
	}

	std::string str = data->args.front();
	max = atoi(str.c_str());
	data->cli->RTSPSetMaxBacklog(max);
}

void Rtsp::MaxClients(struct Data *data)
{
	int max = 0;
	if (data->args.size() == 0)
	{
		int ret = data->cli->RTSPGetMaxClients();
		printf("Max: %d\n", ret);
		return;
	}

	std::string str = data->args.front();
	max = atoi(str.c_str());
	data->cli->RTSPSetMaxClients(max);
}

void Rtsp::Port(struct Data *data)
{
	int port = 0;
	if (data->args.size() == 0)
	{
		int ret = data->cli->RTSPGetPort();
		printf("Port: %d\n", ret);
		return;
	}

	std::string str = data->args.front();
	port = atoi(str.c_str());
	data->cli->RTSPSetPort(port);
}

void Rtsp::KickAll(struct Data *data)
{
	data->cli->RTSPKickAll();
}

void Rtsp::Count(struct Data *data)
{
	int ret = data->cli->RTSPGetClientCount();
	printf("Count: %d\n", ret);
}

void Rtsp::Help(struct Data *data)
{
	DumpHelp(Ops);
}

void Rtsp::Process(struct Data *data)
{
	data->ops = Ops;
	::Process(data);
}

