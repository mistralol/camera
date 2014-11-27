
include Makefile.Rules

SRCS=main.cpp \
	RTSPServer.cpp \
	RTSPServerCleanup.cpp \
	CameraHandler.cpp \
	Platform/Platform.cpp \
	Platform/PlatformBase.cpp \
	Platform/Example/Example.cpp
	

OBJS=$(SRCS:%.cpp=%.o)


all: camera-server

%.o: %.cpp Makefile
	$(CPP) $(CPP_OPTS) $(INCLUDE) -o $@ -c $<

camera-server: $(OBJS) .depend
	$(CPP) $(OBJS) $(LIBS) -o $@

dep: .depend

.depend: $(SRCS)
	$(MKDEP) $(INCLUDE) $(SRCS) > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif


clean:
	$(RM) $(OBJS) *.o camera-server .depend



