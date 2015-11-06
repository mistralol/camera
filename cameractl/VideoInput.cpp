
#include <cameractl.h>

static struct Operations Ops[] = {
	{"help", VideoInput::Help, "Shows this help"},
	{"count", VideoInput::Count, "Show number of video inputs avilable"},
	{"enabled", VideoInput::Enabled,  "enabled <input> (<enabled>) to enable / disable an input"},
	{"supported", VideoInput::Supported, "supported <input> Show supported config"},
	{"config", VideoInput::Config, "config <input> Show current config"},
	{NULL, NULL }
};

void VideoInput::Config(struct Data *data)
{
	if (data->args.size() < 1)
	{
		fprintf(stderr, "Missing paramater 'input'\n");
		return;
	}

	std::string szinput = data->args.front();
	data->args.pop_front();
	int input = atoi(szinput.c_str());

	VideoInputConfig config = data->cli->VideoInputGetConfig(input);
	printf("%s\n", config.ToStr().c_str());
}

void VideoInput::Supported(struct Data *data)
{
	if (data->args.size() < 1)
	{
		fprintf(stderr, "Missing paramater 'input'\n");
		return;
	}

	std::string szinput = data->args.front();
	data->args.pop_front();
	int input = atoi(szinput.c_str());

	VideoInputSupported supported = data->cli->VideoInputGetSupported(input);
	std::vector<std::string> vec = supported.ToStrV();
	for(std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++)
	{
		printf("%s\n", it->c_str());
	}

}

void VideoInput::Enabled(struct Data *data)
{
	if (data->args.size() < 1)
	{
		fprintf(stderr, "Missing paramater 'input'\n");
		return;
	}

	std::string szinput = data->args.front();
	data->args.pop_front();
	int input = atoi(szinput.c_str());

	int enabled = 0;
	if (data->args.size() == 0)
	{
		int ret = data->cli->VideoInputGetEnabled(input);
		printf("Enabled: %d\n", ret);
		return;
	}

	std::string str = data->args.front();
	if (str == "1")
		enabled = true;
	data->cli->VideoInputSetEnabled(input, enabled);
}

void VideoInput::Count(struct Data *data)
{
	int ret = data->cli->VideoInputCount();
	printf("Count: %d\n", ret);
}

void VideoInput::Help(struct Data *data)
{
	DumpHelp(Ops);
}

void VideoInput::Process(struct Data *data)
{
	data->ops = Ops;
	::Process(data);
}
