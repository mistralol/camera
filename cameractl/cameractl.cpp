
#include <cameractl.h>

int DumpHelp(struct Operations *ops)
{
	int i =0;
	printf("Avilable Commands\n");
	while(ops->op != NULL)
	{
		printf("\t%s\n", ops->op);
		ops++;
	}
	return 0;
}

int Process(struct Data *data)
{
	int i =0;
	
	if (data->args.size() == 0)
	{
		fprintf(stderr, "Missing paramater try adding help for more information\n");
		return 0;
	}
	data->command = data->args.front();
	data->args.pop_front();
	
	struct Operations *ops = data->ops;
	while(ops->op != NULL)
	{
		if (data->command == ops->op)
			return ops->func(data);

		ops++;
	}
	
	fprintf(stderr, "No Such Command '%s'\n", data->command.c_str());
	return -1;
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

	struct Data data = {NULL, &cli, command, args};
	int ret = Misc::Process(&data);
	printf("status: %d\n", ret);

	cli.Disconnect();

	return ret;
}

