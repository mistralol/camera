
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

int Rtsp::MaxBacklog(struct Data *data)
{
	int max = 0;
	if (data->args.size() == 0)
	{
		int ret = data->cli->RTSPGetMaxBacklog(&max);
		if (ret < 0)
			return ret;
		return max;
	}

	std::string str = data->args.front();
	max = atoi(str.c_str());
	int ret = data->cli->RTSPSetMaxBacklog(max);
	if (ret < 0)
		return ret;
	return max;
}

int Rtsp::MaxClients(struct Data *data)
{
	int max = 0;
	if (data->args.size() == 0)
	{
		int ret = data->cli->RTSPGetMaxClients(&max);
		if (ret < 0)
			return ret;
		return max;
	}

	std::string str = data->args.front();
	max = atoi(str.c_str());
	int ret = data->cli->RTSPSetMaxClients(max);
	if (ret < 0)
		return ret;
	return max;
}

int Rtsp::Port(struct Data *data)
{
	int port = 0;
	if (data->args.size() == 0)
	{
		int ret = data->cli->RTSPGetPort(&port);
		if (ret < 0)
			return ret;
		return port;
	}

	std::string str = data->args.front();
	port = atoi(str.c_str());
	int ret = data->cli->RTSPSetPort(port);
	if (ret < 0)
		return ret;
	return port;
}

int Rtsp::KickAll(struct Data *data)
{
	return data->cli->RTSPKickAll();
}

int Rtsp::Count(struct Data *data)
{
	int value = 0;
	int ret = data->cli->RTSPGetClientCount(&value);
	if (ret < 0)
		return ret;
	return value;
}

int Rtsp::Help(struct Data *data)
{
	return DumpHelp(Ops);
}

int Rtsp::Process(struct Data *data)
{
	data->ops = Ops;
	return ::Process(data);
}

