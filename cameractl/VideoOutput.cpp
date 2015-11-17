
#include <cameractl.h>

static struct Operations Ops[] = {
	{"help", VideoOutput::Help, "Shows this help"},
	{"count", VideoOutput::Count, "Show number of video inputs avilable"},
	{"supported", VideoOutput::Supported, "Show support information for <output>"},
	{"tourlist", VideoOutput::TourList, "Get list of tours"},
	{"tourget", VideoOutput::TourGet, "Get tour information"},
	{"touradd", VideoOutput::TourAdd, "Add new tour"},
	{"tourupdate", VideoOutput::TourUpdate, "Update tour"},
	{"tourremove", VideoOutput::TourRemove, "Remove tour"},
	{"tourexists", VideoOutput::TourExists, "Does a tour exist"},
	{NULL, NULL }
};

void VideoOutput::Count(struct Data *data)
{
	int ret = data->cli->VideoOutputCount();
	printf("Count: %d\n", ret);
}

void VideoOutput::Supported(struct Data *data)
{
	if (data->args.size() < 1)
	{
		fprintf(stderr, "Missing paramater 'output'\n");
		return;
	}

	std::string szoutput = data->args.front();
	data->args.pop_front();
	int output = atoi(szoutput.c_str());
	
	VideoOutputSupported info = data->cli->VideoOutputGetSupported(output);
	std::vector<std::string> lst = info.ToStrV();
	for(auto it = lst.begin(); it != lst.end(); it++)
	{
		printf("%s\n", it->c_str());
	}
}

void VideoOutput::TourList(struct Data *data)
{
	std::vector<std::string> lst = data->cli->VideoOutputTourList();
	for(auto it = lst.begin(); it != lst.end(); it++)
	{
		printf("%s\n", it->c_str());
	}
}

void VideoOutput::TourGet(struct Data *data)
{
	//VideoOutputTour VideoOutputTourGet(std::string name);

}

void VideoOutput::TourAdd(struct Data *data)
{
	//void VideoOutputTourAdd(VideoOutputTour *tour);
}

void VideoOutput::TourUpdate(struct Data *data)
{
	//void VideoOutputTourUpdate(VideoOutputTour *tour);
}

void VideoOutput::TourRemove(struct Data *data)
{
	if (data->args.size() < 1)
	{
		fprintf(stderr, "Missing paramater 'output'\n");
		return;
	}

	std::string name = data->args.front();
	data->args.pop_front();
	
	data->cli->VideoOutputTourRemove(name);
}

void VideoOutput::TourExists(struct Data *data)
{
	if (data->args.size() < 1)
	{
		fprintf(stderr, "Missing paramater 'output'\n");
		return;
	}

	std::string name = data->args.front();
	data->args.pop_front();
	
	int ret = data->cli->VideoOutputTourExists(name);
	printf("status: %d\n", ret);
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


