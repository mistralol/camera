
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <getopt.h>

#include <list>
#include <map>
#include <string>

#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

#include <json/json.h>

#include <liblogger.h>

using namespace liblogger;

#include <libclientserver.h>

#include <Version.h>
#include <Debug.h>

#include <RTSPServer.h>
#include <RTSPServerCleanup.h>

#include <UserItem.h>
#include <User.h>
#include <Group.h>

#include <IPipeline.h>
#include <PipelineBasic.h>

#include <GstUtil.h>

#include <VideoInputSupported.h>
#include <VideoInputConfig.h>

#include <PlatformBase.h>

#include <WebServer.h>

#include <Config.h>
#include <CameraHandler.h>
#include <CameraServer.h>
#include <System.h>

extern Timers *CameraTimers;



