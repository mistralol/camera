
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
	fprintf(fp, " -l --log               Always log to stdout\n");
	fprintf(fp, "\n");
	fprintf(fp, "\n");
}

class SigHandler : public ISignalHandler
{
	public:
		SigHandler()
		{
			m_Server = NULL;
		}

		~SigHandler()
		{

		}

		void SetServer(CameraServer *Server)
		{
			m_Server = Server;
		}

		void SigHUP(const siginfo_t *info)
		{
			LogInfo("SigHandler::SigHUP");
			LoggerRotate();
		}

		void SigTerm(const siginfo_t *info)
		{
			LogInfo("SigHandler::SigTerm");
			if (m_Server)
			{
				LogInfo("SigTerm::SigTerm - Telling server to quit");
				m_Server->Quit();
			}
			else
			{
				LogError("SigTerm::SigTerm - No server to tell to quit");
			}
		}

		void SigUser1(const siginfo_t *info)
		{
			CameraServer::StatsDump();
		}

		void SigUser2(const siginfo_t *info)
		{
			CameraServer::StatsReset();
		}

		void SigPipe(const siginfo_t *info)
		{
			//Just ignored
		}

	private:
		CameraServer *m_Server;

};

int main(int argc, char **argv)
{
	LogManager::Init();
	CameraServer *Server = NULL;
	SigHandler SHandler;
	SignalHandler Signals = SignalHandler(&SHandler);
	ServerManager *Manager = NULL;
	std::string LocSocket = "/tmp/CameraServer";
	std::string LocPidFile = "";
	std::string DefPlatform = "Example";
	std::string CfgFile = "Config.json";
	PIDFile *PidFile = NULL;
	bool AlwaysLog = false;
	SetUid ProcessPerms;

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
		{"log", 0, 0, 'l'},
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
			case 'l':
				AlwaysLog = true;
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

	//Find out if we are setuid enabled
	if (ProcessPerms.Init() == false)
	{
		LogError("Failed to set process permissions using suid");
	}
	else
	{
		//grab some extra privs then drops user again back to what it was
		do {
			UserID user = UserID();
			if (user.Up(0) < 0)
				LogError("UserID Failed to set 0");
			LogInfo("Setting CAP_NET_BIND_SERVICE");
			int ret = Caps::SetCap(CAP_NET_BIND_SERVICE);
			if (ret < 0)
				LogError("Cannot SetCap CAP_NET_BIND_SERVICE - %s", Errno::ToStr(abs(ret)).c_str() );
		} while(0);
	}


	//Init Handler (This is the "system" init call)
	Server = new CameraServer();
	Manager = new ServerManager(Server);
	Signals.Block(); //Switch off signals during startup
	SHandler.SetServer(Server);
	Server->Init(DefPlatform, CfgFile);

	//Start Our Local Services
	LogDebug("Service Listen On: %s", LocSocket.c_str());
	ServerUnix Unix(LocSocket);
	Manager->ServerAdd(&Unix);
	Signals.UnBlock(); //Accept signals again
	Server->Wait();
	Manager->ServerRemove(&Unix);

	LogInfo("Cleanup");

	Signals.Block();
	SHandler.SetServer(NULL);
	Signals.UnBlock();

	//Cleanup!
	delete Server;
	delete Manager;
	delete PidFile;
	LogManager::RemoveAll(true);
	return 0;
}

