
CPP=g++
CPP_OPTS+=-Wall -ggdb -std=c++11
MKDEP=gcc -E -MM
INCLUDE+=-I../libcameraclient
LIBS+=../build/libcameraclient.a 
DEPS+=../build/libcameraclient.a 

#liblogger
INCLUDE+=$(shell pkg-config --cflags liblogger)
LIBS+=$(shell pkg-config --libs liblogger)

INCLUDE+=$(shell pkg-config --clfags libclientserver)
LIBS+=$(shell pkg-config --libs libclientserver)

LIBS+=-pthread

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
	WebServerSetEnabled.cpp \
	WebServerGetEnabled.cpp \
	WebServerGetPort.cpp \
	WebServerSetPort.cpp \
	WebServerRestart.cpp \
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

