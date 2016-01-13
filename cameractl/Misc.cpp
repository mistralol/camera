
#include <cameractl.h>

static struct Operations Ops[] = {
	{"debug", Misc::Debug, "Get / Set Debug"},
	{"help", Misc::Help, "Shows this help"},
	{"ping", Misc::Ping, "Sends a ping to the camera server"},
	{"version", Misc::Version, "Returns version of the camera server"},
	{"rtsp", Rtsp::Process, "Process rtsp commands"},
	{"system", System::Process, "Process sytem wide commands use system help for more information"},
	{"user", User::Process, "Process comands for the user database"},
	{"videoinput", VideoInput::Process, "Process video input commands"},
	{"videooutput", VideoOutput::Process, "Process video output commands"},
	{"webserver", WebServer::Process, "Process webserver commands"},
	{"webstream", WebStream::Process, "Process webstream comands"},
	{"gpiooutput", GPIOOutput::Process, "Process GPIO Output commands"},
	{"quit", Misc::Quit, "Tells camera server to quit and exit"},
	{"benchmark", Misc::BenchMark, "Perform an api benchmark"},
	{NULL, NULL }
};

void Misc::Debug(struct Data *data)
{
	int enabled = 0;
	if (data->args.size() == 0)
	{
		bool value = data->cli->DebugGetEnabled();
		printf("%d", value);
	}

	std::string str = data->args.front();
	if (str == "1")
		enabled = true;
	data->cli->DebugSetEnabled(enabled);
}

void Misc::Ping(struct Data *data)
{
	data->cli->Ping();
}

void Misc::BenchMark(struct Data *data)
{
	for(int i=0;i<1000;i++)
	{
		data->cli->Ping();
	}
	printf("Done 1000 Calls\n");
}

void Misc::Version(struct Data *data)
{
	std::string version = data->cli->Version();
	printf("%s\n", version.c_str());
}

void Misc::Quit(struct Data *data)
{
	data->cli->Quit();
}

void Misc::Help(struct Data *data)
{
	DumpHelp(Ops);
}

void Misc::Process(struct Data *data)
{
	data->ops = Ops;
	::Process(data);
}

