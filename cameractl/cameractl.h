
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <list>

#include <json/json.h>

#include <libclientserver.h>

#include <Misc.h>
#include <System.h>
#include <WebServer.h>
#include <Rtsp.h>
#include <VideoInput.h>
#include <VideoInputConfig.h>
#include <VideoInputSupported.h>

#include <CameraClient.h>

struct Operations {
	const char *op;
	int (*func) (struct Data *data);
	const char *desc;
};

struct Data
{
	struct Operations *ops;
	CameraClient *cli;
	std::string command;
	std::list<std::string> args;
};

extern int DumpHelp(struct Operations *ops);
extern int Process(struct Data *data);

