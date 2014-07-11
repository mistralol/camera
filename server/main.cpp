
#include <main.h>

int main(int argc, char **argv)
{
	LogManager::Init();
	RTSPServer *RServer = NULL;
	RTSPHandler Handler;
	ServerManager Manager(&Handler);
	ServerUnix Unix("/tmp/CameraServer");

	//Add Logging
	if (isatty(fileno(stdout)) == 1)
	{
		LogManager::Add(new LogStdout());
	}


	//Start RTSP Server
	gst_init (&argc, &argv);
	RServer = new RTSPServer();

	//Load Config
	LogDebug("Adding PipeLine");
	RServer->PipelineAdd("/test", "( videotestsrc ! x264enc ! rtph264pay name=pay0 pt=96 )");

	//Start Our Local Services
	LogDebug("main - Starting Services");
	Manager.ServerAdd(&Unix);
	Handler.Wait();
	Manager.ServerRemove(&Unix);

	LogInfo("RTSP Server Exiting");

	//Cleanup!
	delete RServer;
	gst_deinit();
	LogManager::RemoveAll(true);
	return 0;
}

