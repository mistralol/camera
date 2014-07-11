
#include <main.h>

void print_help(FILE *fp, const char *app)
{
	fprintf(fp, "Usage: %s <options>\n", app);
	fprintf(fp, "\n");
	fprintf(fp, " -h --help              Print this help and exit\n");
	fprintf(fp, " -p --pid <filename>    Use the filename as pid file\n");
	fprintf(fp, " -s --socket <filename> Use the filename as a unix socket file for services\n");
	fprintf(fp, "\n");
	fprintf(fp, "\n");
}

int main(int argc, char **argv)
{
	LogManager::Init();
	RTSPServer *RServer = NULL;
	RTSPHandler Handler;
	ServerManager Manager(&Handler);
	std::string LocSocket = "/tmp/CameraServer";
	std::string LocPidFile = "";
	PIDFile *PidFile = NULL;
	bool AlwaysLog = false;

	const char *opts = "h";
	int longindex = 0;
	int c = 0;
	struct option loptions[]
	{
		{"help", 0, 0, 'h'},
		{"pid", 1, 0, 'p'},
		{"socket", 1, 0, 's'},
		{0, 0, 0, 0}
	};

	while( (c = getopt_long(argc, argv, opts, loptions, &longindex)) >= 0)
	{
		switch(c)
		{
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
			LogError("Cannot Create Pid File - %s", LocPidFile.c_str());
			exit(EXIT_FAILURE);
		}
		LogInfo("Created PIDFile: %s", LocPidFile.c_str());
	}

	//Start RTSP Server
	gst_init (&argc, &argv);
	RServer = new RTSPServer();

	//Load Config
	LogDebug("Adding PipeLine");
	RServer->PipelineAdd("/test", "( videotestsrc ! x264enc ! rtph264pay name=pay0 pt=96 )");

	//Start Our Local Services
	LogDebug("Serverice Listen On: %s", LocSocket.c_str());
	ServerUnix Unix(LocSocket);
	Manager.ServerAdd(&Unix);
	Handler.Wait();
	Manager.ServerRemove(&Unix);

	LogInfo("RTSP Server Exiting");

	//Cleanup!
	delete RServer;
	delete PidFile;
	gst_deinit();
	LogManager::RemoveAll(true);
	return 0;
}

