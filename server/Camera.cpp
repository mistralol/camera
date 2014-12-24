
#include <Camera.h>

void print_help(FILE *fp, const char *app)
{
	fprintf(fp, "Usage: %s <options>\n", app);
	fprintf(fp, "\n");
	fprintf(fp, " -h --help              Print this help and exit\n");
	fprintf(fp, "\n");
	fprintf(fp, " -c --config <filename> Use the filename as config file location\n");
	fprintf(fp, " -p --pid <filename>    Use the filename as pid file\n");
	fprintf(fp, " -s --socket <filename> Use the filename as a unix socket file for services\n");
	fprintf(fp, "\n");
	fprintf(fp, "\n");
}

int main(int argc, char **argv)
{
	LogManager::Init();
	CameraHandler Handler;
	ServerManager Manager(&Handler);
	std::string LocSocket = "/tmp/CameraServer";
	std::string LocPidFile = "";
	std::string DefPlatform = "Example";
	std::string CfgFile = "Config.json";
	PIDFile *PidFile = NULL;
	bool AlwaysLog = false;

	const char *opts = "h";
	int longindex = 0;
	int c = 0;
	struct option loptions[]
	{
		{"config", 1, 0, 'c'},
		{"platform", 1, 0, 'e'},
		{"help", 0, 0, 'h'},
		{"pid", 1, 0, 'p'},
		{"socket", 1, 0, 's'},
		{0, 0, 0, 0}
	};

	while( (c = getopt_long(argc, argv, opts, loptions, &longindex)) >= 0)
	{
		switch(c)
		{
			case 'c':
				CfgFile = optarg;
				break;
			case 'e':
				DefPlatform = optarg;
				break;
			case 'h':
				print_help(stdout, argv[0]);
				exit(EXIT_SUCCESS);
				break;
			case 'p':
				LocPidFile = optarg;
				break;
			case 's':
				LocSocket = optarg;
				break;
			default:
				break;
		}
	}

	//Deal with some annoying signals
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
	{
		printf("signal() failed: %s\n", strerror(errno));
		abort();
	}

	//Add Logging
	if (isatty(fileno(stdout)) == 1 || AlwaysLog == true)
	{
		LogManager::Add(new LogStdout());
	}

	//Setup PID File
	if (LocPidFile != "")
	{
		PidFile = new PIDFile(LocPidFile);
		if (PidFile->Create() == false)
		{
			LogCritical("Cannot Create Pid File - %s", LocPidFile.c_str());
			exit(EXIT_FAILURE);
		}
		LogInfo("Created PIDFile: %s", LocPidFile.c_str());
	}

	//Init Handler (This is the "system" init call)
	Handler.Init(DefPlatform, CfgFile);

	//Start Our Local Services
	LogDebug("Service Listen On: %s", LocSocket.c_str());
	ServerUnix Unix(LocSocket);
	Manager.ServerAdd(&Unix);
	Handler.Wait();
	Manager.ServerRemove(&Unix);

	//Cleanup!
	delete PidFile;
	LogManager::RemoveAll(true);
	return 0;
}

