
CPP=g++
CPP_OPTS+=-Wall -ggdb -std=c++11
MKDEP=gcc -E -MM
INCLUDE+=-I../libcameraclient
INCLUDE+=-I../../libclientserver/include
INCLUDE+=-I../../liblogger/include
LIBS+=../libcameraclient/libcameraclient.a ../../liblogger/liblogger.a ../../libclientserver/libclientserver.a -pthread
DEPS+=../libcameraclient/libcameraclient.a ../../liblogger/liblogger.a ../../libclientserver/libclientserver.a

SRCS=Quit.cpp \
	Ping.cpp \
	RTSPGetMaxBacklog.cpp \
	RTSPSetMaxBacklog.cpp \
	RTSPGetClientCount.cpp \
	RTSPSetMaxClients.cpp \
	RTSPGetMaxClients.cpp

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

