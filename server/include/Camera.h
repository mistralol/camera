
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

#include <RTSPServer.h>
#include <RTSPServerCleanup.h>

#include <IPipeline.h>
#include <PipelineBasic.h>

#include <GstUtil.h>

#include <VideoSupported.h>

#include <Platform/PlatformBase.h>
#include <Platform/Platform.h>

#include <Platform/Example.h>

#include <Config.h>
#include <CameraHandler.h>


