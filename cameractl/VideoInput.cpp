
#include <cameractl.h>

static struct Operations Ops[] = {
	{"help", VideoInput::Help, "Shows this help"},
	{"count", VideoInput::Count, "Show number of video inputs avilable"},
	{"enabled", VideoInput::Enabled,  "enabled <input> (<enabled>) to enable / disable an input"},
	{NULL, NULL }
};

int VideoInput::Enabled(struct Data *data)
{
	if (data->args.size() < 1)
	{
		fprintf(stderr, "Missing paramater 'input'\n");
		return -1;
	}

	std::string szinput = data->args.front();
	data->args.pop_front();
	int input = atoi(szinput.c_str());

	int enabled = 0;
	if (data->args.size() == 0)
	{
		int ret = data->cli->VideoInputGetEnabled(input, &enabled);
		if (ret < 0)
			return ret;
		return enabled;
	}

	std::string str = data->args.front();
	if (str == "1")
		enabled = true;
	int ret = data->cli->VideoInputSetEnabled(input, enabled);
	if (ret < 0)
		return ret;
	return enabled;
}

int VideoInput::Count(struct Data *data)
{
	int value = 0;
	int ret = data->cli->VideoInputCount(&value);
	if (ret < 0)
		return ret;
	return value;
}

int VideoInput::Help(struct Data *data)
{
	return DumpHelp(Ops);
}

int VideoInput::Process(struct Data *data)
{
	data->ops = Ops;
	return ::Process(data);
}
