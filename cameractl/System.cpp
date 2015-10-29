
#include <cameractl.h>

static struct Operations Ops[] = {
	{"help", System::Help, "Shows this help"},
	{"reboot", System::Reboot, "reboot the system"},
	{"shutdown", System::Shutdown, "shutdown the system"},
	{NULL, NULL }
};

void System::Reboot(struct Data *data)
{
	data->cli->SystemReboot();
}

void System::Shutdown(struct Data *data)
{
	data->cli->SystemShutdown();
}

void System::Help(struct Data *data)
{
	DumpHelp(Ops);
}

void System::Process(struct Data *data)
{
	data->ops = Ops;
	::Process(data);
}

