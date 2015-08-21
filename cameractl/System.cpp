
#include <cameractl.h>

static struct Operations Ops[] = {
	{"help", System::Help, "Shows this help"},
	{"reboot", System::Reboot, "reboot the system"},
	{"shutdown", System::Shutdown, "shutdown the system"},
	{NULL, NULL }
};

int System::Reboot(struct Data *data)
{
	return data->cli->SystemReboot();
}

int System::Shutdown(struct Data *data)
{
	return data->cli->SystemShutdown();
}

int System::Help(struct Data *data)
{
	return DumpHelp(Ops);
}

int System::Process(struct Data *data)
{
	data->ops = Ops;
	return ::Process(data);
}

