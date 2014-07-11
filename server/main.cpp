
#include <main.h>

int main(int argc, char **argv)
{
	LogManager::Init();
	RTSPServer *RServer = NULL;

	if (isatty(fileno(stdout)) == 1)
	{
		LogManager::Add(new LogStdout());
	}

	gst_init (&argc, &argv);
	RServer = new RTSPServer();


	LogDebug("Adding PipeLine");
	RServer->PipelineAdd("/test", "( videotestsrc ! x264enc ! rtph264pay name=pay0 pt=96 )");

	while(1)
	{
		LogInfo("Running - Total Sessions: %d", RServer->SessionsCount());
		sleep(1);
	}

	LogInfo("RTSP Server Exiting");

	delete RServer;
	gst_deinit();
	LogManager::RemoveAll(true);
	return 0;
}

