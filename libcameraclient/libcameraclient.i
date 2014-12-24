%module CameraClientWrapper
%{
using namespace std;
#include <stdint.h>
#include <libclientserver.h>
#include "libcameraclient.h"
%}

%include <std_string.i>
%include libcameraclient.h

