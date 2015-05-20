
INCLUDE+=-I. -I../../libclientserver/include
LIBS+=../../libclientserver/libclientserver.a

#liblogger
INCLUDE+=$(shell pkg-config --cflags liblogger)
LIBS+=$(shell pkg-config --libs liblogger)

CPP=g++
CPP_OPTS+=-Wall -ggdb -std=c++11 -fpic
MKDEP=gcc -E -MM

SRCS=	CameraClient.cpp

OBJS=$(SRCS:%.cpp=%.o)

all: ../build/libcameraclient.so ../build/libcameraclient.a ../build/libcameraclient_python.so ../build/libcameraclient_csharp.so ../build/CameraClient.dll

%.o: %.cpp Makefile
	$(CPP) $(CPP_OPTS) $(INCLUDE) -o $@ -c $<

../build/libcameraclient.so: $(OBJS) .depend
	$(CPP) -shared $(OBJS) $(LIBS) -o $@

../build/libcameraclient.a: $(OBJS) .depend
	$(AR) cru $@ $(OBJS)

#Python Wrapper
libcameraclient_python.cxx: $(SRCS) libcameraclient.i .depend
	swig -Wall -c++ -o libcameraclient_python.cxx -python libcameraclient.i

../build/libcameraclient_python.so: libcameraclient_python.cxx ../build/libcameraclient.a
	$(CPP) -shared -ggdb -Wall -fPIC libcameraclient_python.cxx ../build/libcameraclient.a $(LIBS) $(INCLUDE) -I/usr/include/python2.7 -o $@
	if [ ! -e ../build/_CameraClientWrapper.so ] ; then ln -s ../build/libcameraclient_python.so ../build/_CameraClientWrapper.so ; fi

#CSharp Wrapper
libcameraclient_csharp.cxx: $(SRCS) libcameraclient.i .depend
	swig -Wall -c++ -o libcameraclient_csharp.cxx -csharp libcameraclient.i

../build/libcameraclient_csharp.so: libcameraclient_csharp.cxx ../build/libcameraclient.a
	$(CPP) -shared -ggdb -Wall -fPIC libcameraclient_csharp.cxx ../build/libcameraclient.a $(LIBS) $(INCLUDE) -o $@
	if [ ! -e ../build/libCameraClientWrapper ] ; then ln -s ../build/libcameraclient_csharp.so ../build/libCameraClientWrapper ; fi

../build/CameraClient.dll: $(SRCS) libcameraclient_csharp.cxx
	gmcs -t:library *.cs -out:../build/CameraClient.dll

dep: .depend

.depend: $(SRCS)
	$(MKDEP) $(INCLUDE) $(SRCS) > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif

clean:
	$(RM) *.o *.so *.cs libcameraclient.a .depend libCameraClientWrapper
	$(RM) ../build/libcameraclient_python.cxx ../build/libcameraclient_csharp.cxx ../build/CameraClientWrapper.py ../build/CameraClientWrapper.pyc


