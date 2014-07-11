
INCLUDE+=-I. -I../../libclientserver/include -I../../liblogger/include
INCLUDE+=`pkg-config --cflags gstreamer-0.10`

LIBS+=../../liblogger/liblogger.a ../../libclientserver/libclientserver.a
LIBS+=`pkg-config --libs gstreamer-0.10`
LIBS+=/usr/lib/libgstrtspserver-0.10.so.0

CPP=g++
CPP_OPTS+=-Wall -ggdb -std=c++11
MKDEP=gcc -E -MM

SRCS=main.cpp \
	RTSPServer.cpp \
	RTSPServerCleanup.cpp \
	RTSPHandler.cpp

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
	$(RM) *.o camera-server .depend



