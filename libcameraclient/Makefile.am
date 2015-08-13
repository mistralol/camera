
BUILT_SOURCES=libcameraclient_csharp.cpp \
		libcameraclient_python.cpp


lib_LTLIBRARIES=libcameraclient.la \
		libcameraclient_csharp.la \
		libcameraclient_python.la

libcameraclient_la_SOURCES=CameraClient.cpp
libcameraclient_la_CXXFLAGS=-I../libcameracommon $(JSON_CFLAGS) $(LIBLOGGER_CFLAGS) $(LIBCLIENTSERVER_CFLAGS)
libcameraclient_la_LDFLAGS=../libcameracommon/.libs/libcameracommon.a $(LIBCLIENTSERVER_LIBS) $(LIBLOGGER_LIBS)

libcameraclient_csharp_la_SOURCES=libcameraclient_csharp.cpp
libcameraclient_csharp_la_CXXFLAGS=-I../libcameracommon $(JSON_CFLAGS) $(LIBLOGGER_CFLAGS) $(LIBCLIENTSERVER_CFLAGS)
libcameraclient_csharp_la_LDFLAGS=$(LIBCLIENTSERVER_LIBS) $(LIBLOGGER_LIBS)

libcameraclient_python_la_SOURCES=libcameraclient_python.cpp
libcameraclient_python_la_CXXFLAGS=-I../libcameracommon $(PYTHON_CFLAGS) $(JSON_CFLAGS) $(LIBLOGGER_CFLAGS) $(LIBCLIENTSERVER_CFLAGS)
libcameraclient_python_la_LDFLAGS=$(LIBCLIENTSERVER_LIBS) $(LIBLOGGER_LIBS)


libcameraclient_csharp.cpp: CameraClient.cpp CameraClient.h CameraClient.i
	swig -Wall -c++ -o libcameraclient_csharp.cpp -csharp CameraClient.i

libcameraclient_python.cpp: CameraClient.cpp CameraClient.h CameraClient.i
	swig -Wall -c++ -o libcameraclient_python.cpp -python CameraClient.i


all-local:
	gmcs -t:library -debug *.cs -out:CameraClient.dll

clean-local:
	$(RM) -f CameraClient.dll
	$(RM) -f libcameraclient_csharp.cpp
	$(RM) -f libcameraclient_python.cpp
	$(RM) -f SWIGType*
	$(RM) -f *.cs *.py



