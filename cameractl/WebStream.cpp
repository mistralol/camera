
#include <cameractl.h>

static struct Operations Ops[] = {
	{"help", WebServer::Help, "Shows this help"},
	{"start", WebStream::Start, "Start a web stream"},
	{NULL, NULL }
};

int WebStream::Start(struct Data *data)
{
	int enabled = 0;
	if (data->args.size() == 0)
	{
		printf("Need a Video paramater\n");
		return -1;
	}

	std::string str = data->args.front();
	WebStreamOptions options;
	options.vinput = atoi(str.c_str());
	return data->cli->WebStreamStart(&options);
}

int WebStream::Help(struct Data *data)
{
	return DumpHelp(Ops);
}

int WebStream::Process(struct Data *data)
{
	data->ops = Ops;
	return ::Process(data);
}

