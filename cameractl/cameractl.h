
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
#include <WebStream.h>
#include <Rtsp.h>
#include <VideoInput.h>
#include <VideoInputConfig.h>
#include <VideoInputSupported.h>
#include <WebStreamType.h>
#include <WebStreamOptions.h>
#include <UserItem.h>
#include <User.h>
#include <GPIOOutput.h>


#include <CameraClient.h>

struct Operations {
	const char *op;
	void (*func) (struct Data *data);
	const char *desc;
};

struct Data
{
	struct Operations *ops;
	CameraClient *cli;
	std::string command;
	std::list<std::string> args;
};

extern void DumpHelp(struct Operations *ops);
extern void Process(struct Data *data);

