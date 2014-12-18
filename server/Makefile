
include Makefile.Rules

SRCS=	Config.cpp \
	Camera.cpp \
	CameraHandler.cpp \
	RTSPServer.cpp \
	RTSPServerCleanup.cpp \
	GstUtil.cpp \
	PipelineBasic.cpp \
	Platform/Platform.cpp \
	Platform/PlatformBase.cpp \
	Platform/Example.cpp

INCS=$(shell find include -type f)	

OBJS=$(SRCS:%.cpp=%.o)


all: camera

%.o: %.cpp Makefile
	$(CPP) $(CPP_OPTS) $(INCLUDE) -o $@ -c $<

camera: $(OBJS) .depend
	$(CPP) $(OBJS) $(LIBS) -o $@

dep: .depend

.depend: $(SRCS) $(INCS)
	$(MKDEP) $(INCLUDE) $(SRCS) > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif


clean:
	$(RM) $(OBJS) *.o camera-server .depend



