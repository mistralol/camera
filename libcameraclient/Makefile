
INCLUDE+=-I. -I../../libclientserver/include -I../../liblogger/include
INCLUDE+=

LIBS+=../../liblogger/liblogger.a ../../libclientserver/libclientserver.a

CPP=g++
CPP_OPTS+=-Wall -ggdb -std=c++11 -fpic
MKDEP=gcc -E -MM

SRCS=	CameraClient.cpp

OBJS=$(SRCS:%.cpp=%.o)

all: libcameraclient.so libcameraclient.a libcameraclient_python.so libcameraclient_csharp.so

%.o: %.cpp Makefile
	$(CPP) $(CPP_OPTS) $(INCLUDE) -o $@ -c $<

libcameraclient.so: $(OBJS) .depend
	$(CPP) -shared $(OBJS) $(LIBS) -o $@

libcameraclient.a: $(OBJS) .depend
	$(AR) cru $@ $(OBJS)

#Python Wrapper
libcameraclient_python.cxx: $(SRCS) libcameraclient.i .depend
	swig -Wall -c++ -o libcameraclient_python.cxx -python libcameraclient.i

libcameraclient_python.so: libcameraclient_python.cxx libcameraclient.a $(LIBS)
	$(CPP) -shared -ggdb -Wall -fPIC libcameraclient_python.cxx libcameraclient.a $(LIBS) $(INCLUDE) -I/usr/include/python2.7 -o $@
	if [ ! -e _CameraClientWrapper.so ] ; then ln -s libcameraclient_python.so _CameraClientWrapper.so ; fi

#CSharp Wrapper
libcameraclient_csharp.cxx: $(SRCS) libcameraclient.i .depend
	swig -Wall -c++ -o libcameraclient_csharp.cxx -csharp libcameraclient.i

libcameraclient_csharp.so: libcameraclient_csharp.cxx libcameraclient.a $(LIBS)
	$(CPP) -shared -ggdb -Wall -fPIC libcameraclient_csharp.cxx libcameraclient.a $(LIBS) $(INCLUDE) -o $@
	if [ ! -e libCameraClientWrapper ] ; then ln -s libcameraclient_csharp.so libCameraClientWrapper ; fi

dep: .depend

.depend: $(SRCS)
	$(MKDEP) $(INCLUDE) $(SRCS) > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif

clean:
	$(RM) *.o *.so *.cs libcameraclient.a .depend libCameraClientWrapper
	$(RM) libcameraclient_python.cxx libcameraclient_csharp.cxx CameraClientWrapper.py CameraClientWrapper.pyc


