
#include <cameractl.h>

static struct Operations Ops[] = {
	{"help", WebServer::Help, "Shows this help"},
	{"start", WebStream::Start, "Start a web stream"},
	{NULL, NULL }
};

void WebStream::Start(struct Data *data)
{
	int enabled = 0;
	if (data->args.size() == 0)
	{
		printf("Need a Video paramater\n");
		return;
	}

	std::string str = data->args.front();
	WebStreamOptions options;
	options.vinput = atoi(str.c_str());
	options.type = MP4;
	int port = data->cli->WebStreamStart(&options);
	printf("Port: %d\n", port);
}

void WebStream::Help(struct Data *data)
{
	DumpHelp(Ops);
}

void WebStream::Process(struct Data *data)
{
	data->ops = Ops;
	::Process(data);
}

