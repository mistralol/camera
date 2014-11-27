
#include <string>

#include "PlatformBase.h"
#include "Platform.h"
#include "Example/Example.h"


PlatformBase *Platform::Create(const std::string str)
{
	if (str == "Example")
		return new Example();

	return NULL;
}


