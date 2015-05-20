
CPP=g++
CPP_OPTS+=-Wall -ggdb -std=c++11
MKDEP=gcc -E -MM
INCLUDE+=-I../libcameraclient
INCLUDE+=-I../../libclientserver/include
LIBS+=../build/libcameraclient.a ../../libclientserver/libclientserver.a -pthread
DEPS+=../build/libcameraclient.a ../../libclientserver/libclientserver.a

#liblogger
INCLUDE+=$(shell pkg-config --cflags liblogger)
LIBS+=$(shell pkg-config --libs liblogger)

SRCS=Quit.cpp \
	Ping.cpp \
	Version.cpp \
	RTSPKickAll.cpp \
	RTSPSetPort.cpp \
	RTSPGetPort.cpp \
	RTSPGetMaxBacklog.cpp \
	RTSPSetMaxBacklog.cpp \
	RTSPGetClientCount.cpp \
	RTSPSetMaxClients.cpp \
	RTSPGetMaxClients.cpp \
	VideoStreamCount.cpp \
	VideoStreamSetEnabled.cpp \
	VideoStreamGetEnabled.cpp \
	SystemReboot.cpp \
	SystemShutdown.cpp

EXES=$(SRCS:%.cpp=%)

all: $(EXES)

%: %.cpp $(DEPS)
	$(CPP) $(CPP_OPTS) $(INCLUDE)  $< -o $@ $(LIBS)

.depend: $(SRCS) $(INCS)
	$(MKDEP) $(INCLUDE) $(SRCS) > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif

clean:
	rm -f $(EXES)

