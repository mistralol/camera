
INCLUDE+=-I. -I../../libclientserver/include -I../../liblogger/include
INCLUDE+=

LIBS+=../../liblogger/liblogger.a ../../libclientserver/libclientserver.a

CPP=g++
CPP_OPTS+=-Wall -ggdb -std=c++11 -fpic
MKDEP=gcc -E -MM

SRCS=	CameraClient.cpp

OBJS=$(SRCS:%.cpp=%.o)

all: libcameraclient.so libcameraclient.a libcameraclient_python.so

%.o: %.cpp Makefile
	$(CPP) $(CPP_OPTS) $(INCLUDE) -o $@ -c $<

libcameraclient.so: $(OBJS) .depend
	$(CPP) -shared $(OBJS) $(LIBS) -o $@

libcameraclient.a: $(OBJS) .depend
	$(AR) cru $@ $(OBJS)

libcameraclient_python.cxx: $(SRCS) libcameraclient.i .depend
	swig -Wall -c++ -o libcameraclient_python.cxx -python libcameraclient.i

libcameraclient_python.so: libcameraclient_python.cxx libcameraclient.a ../../libclientserver/libclientserver.a
	$(CPP) -shared -ggdb -Wall -fPIC libcameraclient_python.cxx libcameraclient.a ../../libclientserver/libclientserver.a $(INCLUDE) -I/usr/include/python2.7 -o $@
	ln -s libcameraclient_python.so _CameraClient.so

dep: .depend

.depend: $(SRCS)
	$(MKDEP) $(INCLUDE) $(SRCS) > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif

clean:
	$(RM) *.o *.so libcameraclient.a .depend
	$(RM) libcameraclient_python.cxx CameraClient.py CameraClient.pyc


