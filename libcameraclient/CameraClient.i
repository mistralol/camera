%module CameraClientWrapper
%{
using namespace std;
#include <stdint.h>
#include <json/json.h>
#include <libclientserver.h>
#include <WebStreamType.h>
#include <WebStreamOptions.h>
#include <VideoInputSupported.h>
#include <VideoInputConfig.h>
#include <VideoOutputSupported.h>
#include <VideoOutputConfig.h>
#include <VideoOutputTourItem.h>
#include <VideoOutputTour.h>
#include <UserItem.h>

#include "CameraClient.h"
%}

%include <std_vector.i>
%include <std_map.i>
%include <std_pair.i>
%include <std_string.i>
%include <typemaps.i>
%include <stl.i>

%apply long { time_t };
%apply int { time_t };

%template(StringVector) std::vector<std::string>;
%template(StringInt) std::vector<int>;

%exception {
try {
	$action
} catch (std::exception e) {
	SWIG_CSharpSetPendingException(SWIG_CSharpApplicationException, e.what());
}
}

%include CameraClient.h
%include VideoInputSupported.h
%include VideoInputConfig.h
%include VideoOutputSupported.h
%include VideoOutputConfig.h
%include VideoOutputTour.h
%include VideoOutputTourItem.h
%include UserItem.h
%include WebStreamType.h
%include WebStreamOptions.h

