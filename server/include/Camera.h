
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

#include <liblogger.h>

using namespace liblogger;

#include <libclientserver.h>

#include <RTSPServer.h>
#include <RTSPServerCleanup.h>
#include <CameraHandler.h>

#include <VideoSupported.h>

#include <Platform/PlatformBase.h>
#include <Platform/Platform.h>

#include <Platform/Example.h>

