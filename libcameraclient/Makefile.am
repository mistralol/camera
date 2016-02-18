
BUILT_SOURCES=libcameraclient_csharp.cpp

lib_LTLIBRARIES=libcameraclient.la \
		libcameraclient_csharp.la

libcameraclient_la_SOURCES=CameraClient.cpp CameraClientException.cpp
libcameraclient_la_CXXFLAGS=-fPIC -I../libcameracommon $(JSON_CFLAGS) $(LIBCLIENTSERVER_CFLAGS)
libcameraclient_la_LDFLAGS=-Wl,-z,defs -L../libcameracommon/.libs -lcameracommon \
		$(JSON_LIBS) \
		$(LIBCLIENTSERVER_LIBS)

libcameraclient_csharp_la_SOURCES=libcameraclient_csharp.cpp
libcameraclient_csharp_la_CXXFLAGS=-ggdb -fPIC -I../libcameracommon $(JSON_CFLAGS) $(LIBCLIENTSERVER_CFLAGS)

libcameraclient_csharp_la_LDFLAGS=-Wl,-z,defs -L.libs -lcameraclient \
		-L../libcameracommon/.libs -lcameracommon \
		$(JSON_LIBS) \
		$(LIBCLIENTSERVER_LIBS)

libcameraclient_csharp.cpp: CameraClient.cpp CameraClient.h CameraClient.i libcameraclient.la
	swig -Wall -c++ -I../libcameracommon -o libcameraclient_csharp.cpp -csharp CameraClient.i

CameraClient.dll: *.cs
	gmcs -t:library -debug *.cs -out:CameraClient.dll

all-local: CameraClient.dll
	if [ ! -e .libs/libCameraClientWrapper.so ] ; then ln -s libcameraclient_csharp.so.0.0.0 .libs/libCameraClientWrapper.so ; fi

clean-local:
	$(RM) -f CameraClient.dll
	$(RM) -f libcameraclient_csharp.cpp
	$(RM) -f SWIGType*
	$(RM) -f *.cs *.py *.mdb
	$(RM) .libs/libCameraClientWrapper.so



