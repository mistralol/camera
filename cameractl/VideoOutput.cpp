
#include <cameractl.h>

static struct Operations Ops[] = {
	{"help", VideoInput::Help, "Shows this help"},
	{"count", VideoInput::Count, "Show number of video inputs avilable"},
	{NULL, NULL }
};

void VideoOutput::Count(struct Data *data)
{
	int ret = data->cli->VideoInputCount();
	printf("Count: %d\n", ret);
}

void VideoOutput::Help(struct Data *data)
{
	DumpHelp(Ops);
}

void VideoOutput::Process(struct Data *data)
{
	data->ops = Ops;
	::Process(data);
}


