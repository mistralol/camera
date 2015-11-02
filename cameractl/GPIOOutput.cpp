#include <cameractl.h>

static struct Operations Ops[] = {
	{"count", GPIOOutput::Count, "Show the number of outputs"},
	{"state", GPIOOutput::State, "Set the state of the output"},
	{"pulse", GPIOOutput::Pulse, "Pulse output in a certain direction"},
	{"help", GPIOOutput::Help, "Shows this help"},
	{NULL, NULL }
};

void GPIOOutput::Count(struct Data *data)
{
	int ret = data->cli->GPIOOutputCount();
	printf("Count: %d\n", ret);
}

void GPIOOutput::State(struct Data *data)
{
	if (data->args.size() == 0)
	{
		fprintf(stderr, "Missing paramater output");
		return;
	}

	if (data->args.size() == 1)
	{
		std::string str = data->args.front();
		data->args.pop_front();
		int output = atoi(str.c_str());
		bool state = data->cli->GPIOOutputGetState(output);
		return;
	}

	if (data->args.size() == 2)
	{
		std::string str1 = data->args.front();
		data->args.pop_front();
		std::string str2 = data->args.front();
		data->args.pop_front();
		int output = atoi(str1.c_str());
		bool state = (bool) atoi(str2.c_str());
		data->cli->GPIOOutputSetState(output, state);
		return;
	}

	fprintf(stderr, "Too many paramaters");
	return;
}

void GPIOOutput::Pulse(struct Data *data)
{
	if (data->args.size() < 3)
	{
		fprintf(stderr, "Usage: <output> <state> <timeout in seconds>");
	}

	if (data->args.size() == 3)
	{
		std::string str1 = data->args.front();
		data->args.pop_front();
		std::string str2 = data->args.front();
		data->args.pop_front();
		std::string str3 = data->args.front();
		data->args.pop_front();
		int output = atoi(str1.c_str());
		bool state = (bool )atoi(str2.c_str());
		struct timespec timeout = {0, 0};
		timeout.tv_sec = atoi(str3.c_str());
		data->cli->GPIOOutputSetState(output, state, &timeout);
		return;
	}

	fprintf(stderr, "Too many paramaters");
	return;
}

void GPIOOutput::Help(struct Data *data)
{
	DumpHelp(Ops);
}

void GPIOOutput::Process(struct Data *data)
{
	data->ops = Ops;
	::Process(data);
}

