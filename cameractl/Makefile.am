

bin_PROGRAMS=cameractl

cameractl_SOURCES=cameractl.cpp \
			Misc.cpp \
			System.cpp \
			WebServer.cpp \
			WebStream.cpp \
			Rtsp.cpp \
			VideoInput.cpp \
			User.cpp
			
cameractl_CXXFLAGS=-I../libcameraclient -I../libcameracommon $(JSON_CFLAGS) $(LIBCLIENTSERVER_CFLAGS)
cameractl_LDFLAGS=../libcameraclient/libcameraclient.la ../libcameracommon/libcameracommon.la $(JSON_LIBS) $(LIBCLIENTSERVER_LIBS) -pthread


