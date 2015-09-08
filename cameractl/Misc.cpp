
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
	{"webserver", WebServer::Process, "Process webserver commands"},
	{"quit", Misc::Quit, "Tells camera server to quit and exit"},
	{NULL, NULL }
};

int Misc::Debug(struct Data *data)
{
	int enabled = 0;
	if (data->args.size() == 0)
	{
		int ret = data->cli->DebugGetEnabled(&enabled);
		if (ret < 0)
			return ret;
		return enabled;
	}

	std::string str = data->args.front();
	if (str == "1")
		enabled = true;
	return data->cli->DebugSetEnabled(enabled);
}

int Misc::Ping(struct Data *data)
{
	return data->cli->Ping();
}

int Misc::Version(struct Data *data)
{
	std::string version = "";
	int ret = data->cli->Version(&version);
	if (ret == 0)
		printf("%s\n", version.c_str());
	return ret;
}

int Misc::Quit(struct Data *data)
{
	int ret = data->cli->Quit();
	printf("status: %d\n", ret);
	if (ret == 0)
		exit(EXIT_SUCCESS);
	return ret;
}

int Misc::Help(struct Data *data)
{
	return DumpHelp(Ops);
}

int Misc::Process(struct Data *data)
{
	data->ops = Ops;
	return ::Process(data);
}

