
AM_CXXFLAGS=-fPIC $(GST_CFLAGS) $(JSON_CFLAGS) $(LIBLOGGER_CFLAGS) $(LIBCLIENTSERVER_CFLAGS) -I../platforms/include

bin_PROGRAMS=camera

BUILT_SOURCES = CameraServerFunctions.i

CameraServerFunctions.i: CameraServerFunctions.gperf
	gperf -tCG CameraServerFunctions.gperf > CameraServerFunctions.i || rm -f CameraServerFunctions.i

camera_SOURCES= Camera.cpp \
		CameraHandler.cpp \
		CameraServer.cpp \
		Config.cpp \
		Debug.cpp \
		GstLogger.cpp \
		PipelineBasic.cpp \
		RTSPServerCleanup.cpp \
		RTSPServer.cpp \
		System.cpp \
		User.cpp \
		UserScopedLock.cpp \
		Version.cpp \
		WebServer.cpp \
		WebStream.cpp \
		WebStreamOptions.cpp \
		WebStreamPipeline.cpp \
		GPIOOutputTimer.cpp \
		UserItem.cpp \
		VideoInputConfig.cpp \
		VideoInputSupported.cpp

camera_LDFLAGS=-rdynamic -Wl,-z,defs \
	$(GST_LIBS) -lgstrtsp-1.0 \
	$(JSON_LIBS) \
	$(LIBCLIENTSERVER_LIBS) \
	$(LIBLOGGER_LIBS) \
	-ldl


