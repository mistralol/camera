

lib_LTLIBRARIES=libcameracommon.la


libcameracommon_la_SOURCES=VideoInputConfig.cpp \
			VideoInputSupported.cpp \
			VideoOutputConfig.cpp \
			VideoOutputTour.cpp \
			VideoOutputTourItem.cpp \
			VideoOutputSupported.cpp \
			WebStreamOptions.cpp

libcameracommon_la_CXXFLAGS=-fPIC $(JSON_CFLAGS)
libcameracommon_la_LDFLAGS=-Wl,-z,defs $(JSON_LIBS)



