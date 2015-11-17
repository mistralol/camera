
#include <cameractl.h>

void DumpHelp(struct Operations *ops)
{
	printf("Avilable Commands\n");
	while(ops->op != NULL)
	{
		printf("\t%s\n", ops->op);
		ops++;
	}
}

void Process(struct Data *data)
{
	
	if (data->args.size() == 0)
	{
		fprintf(stderr, "Missing paramater try adding help for more information\n");
		throw(std::runtime_error("Missing paramater"));
	}
	data->command = data->args.front();
	data->args.pop_front();
	
	struct Operations *ops = data->ops;
	while(ops->op != NULL)
	{
		if (data->command == ops->op)
		{
			ops->func(data);
			return;
		}
		ops++;
	}
	
	fprintf(stderr, "No Such Command '%s'\n", data->command.c_str());
}

int main(int argc, char **argv)
{
	CameraClient cli;

	if (argc < 2)
	{
		fprintf(stderr, "Invalid usage\n");
		fprintf(stderr, "Mayby try: %s help\n", argv[0]);
		exit(EXIT_FAILURE);
	}
		
	if (cli.Connect("unix:/tmp/CameraServer") == false)
	{
		fprintf(stderr, "Cannot connect to camera server");
		exit(EXIT_FAILURE);
	}

	std::string command = "";
	std::list<std::string> args;
	for(int j = 1;j<argc;j++)
		args.push_back(argv[j]);

	try
	{
		struct Data data = {NULL, &cli, command, args};
		Misc::Process(&data);
	} catch(std::exception ex)
	{
		fprintf(stderr, "Exception: %s\n", ex.what());
	}
	cli.Disconnect();
}

