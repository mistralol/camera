
include Makefile.Rules

SRCS=	Config.cpp \
	Camera.cpp \
	CameraHandler.cpp \
	CameraServer.cpp \
	RTSPServer.cpp \
	RTSPServerCleanup.cpp \
	GstUtil.cpp \
	PipelineBasic.cpp \
	VideoStreamSupported.cpp \
	VideoStreamConfig.cpp \
	Platform/Platform.cpp \
	Platform/PlatformBase.cpp \
	Platform/Example.cpp \
	Version.cpp

INCS=$(shell find include -type f)	

OBJS=$(SRCS:%.cpp=%.o)


all: ../build/camera


CameraServerFunctions.i: CameraServerFunctions.gperf
	gperf -tCG CameraServerFunctions.gperf > CameraServerFunctions.i

%.o: %.cpp Makefile CameraServerFunctions.i
	$(CPP) $(CPP_OPTS) $(INCLUDE) -o $@ -c $<

../build/camera: $(OBJS) .depend
	$(CPP) $(OBJS) $(LIBS) -o $@

dep: .depend

.depend: $(SRCS) $(INCS)
	$(MKDEP) $(INCLUDE) $(SRCS) > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif


clean:
	$(RM) $(OBJS) *.o CameraServerFunctions.i ../build/camera .depend



