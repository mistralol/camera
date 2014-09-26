
INCLUDE+=-I. -I../../libclientserver/include -I../../liblogger/include
INCLUDE+=`pkg-config --cflags gstreamer-1.0` -I/usr/local/include/gstreamer-1.0

LIBS+=../../liblogger/liblogger.a ../../libclientserver/libclientserver.a
LIBS+=`pkg-config --libs gstreamer-1.0`
LIBS+=/usr/local/lib/libgstrtspserver-1.0.so.0
LIBS+=-pthread

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



