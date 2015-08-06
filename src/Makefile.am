
AM_CXXFLAGS=$(GST_CFLAGS) $(JSON_CFLAGS) $(LIBLOGGER_CFLAGS) $(LIBCLIENTSERVER_CFLAGS) -I../platforms/include

bin_PROGRAMS=camera

BUILT_SOURCES = CameraServerFunctions.i

CameraServerFunctions.i: CameraServerFunctions.gperf
	gperf -tCG CameraServerFunctions.gperf > CameraServerFunctions.i || rm -f CameraServerFunctions.i


camera_SOURCES= Camera.cpp \
		CameraHandler.cpp \
		CameraServer.cpp \
		Config.cpp \
		Debug.cpp \
		Group.cpp \
		GstUtil.cpp \
		PipelineBasic.cpp \
		RTSPServerCleanup.cpp \
		RTSPServer.cpp \
		System.cpp \
		User.cpp \
		Version.cpp \
		VideoStreamConfig.cpp \
		VideoStreamSupported.cpp \
		WebServer.cpp

camera_LDFLAGS=$(GST_LIBS) $(JSON_LIBS) $(LIBCLIENTSERVER_LIBS) $(LIBLOGGER_LIBS)

