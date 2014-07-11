
INCLUDE+=-I. -I../../libclientserver/include -I../../liblogger/include
INCLUDE+=

LIBS+=../../liblogger/liblogger.a ../../libclientserver/libclientserver.a

CPP=g++
CPP_OPTS+=-Wall -ggdb -std=c++11 -fpic
MKDEP=gcc -E -MM

SRCS=	CameraClient.cpp

OBJS=$(SRCS:%.cpp=%.o)

all: libcameraclient.so libcameraclient.a

%.o: %.cpp Makefile
	$(CPP) $(CPP_OPTS) $(INCLUDE) -o $@ -c $<

libcameraclient.so: $(OBJS) .depend
	$(CPP) -shared $(OBJS) $(LIBS) -o $@

libcameraclient.a: $(OBJS) .depend
	$(AR) cru $@ $(OBJS)

dep: .depend

.depend: $(SRCS)
	$(MKDEP) $(INCLUDE) $(SRCS) > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif

clean:
	$(RM) *.o libcameraclient.so libcameraclient.a .depend


