
#include <main.h>

int main(int argc, char **argv)
{
	RTSPServer *RServer = NULL;

	gst_init (&argc, &argv);
	RServer = new RTSPServer();


	printf("Adding PipeLine");
	RServer->PipelineAdd("/test", "( videotestsrc ! x264enc ! rtph264pay name=pay0 pt=96 )");

	while(1)
	{
		printf("Running - Total Sessions: %d\n", RServer->SessionsCount());
		sleep(1);
	}

	printf("Exiting\n");

	delete RServer;
	gst_deinit();
	return 0;
}

