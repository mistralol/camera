
#include <Camera.h>

PlatformBase *Platform::Create(const std::string str)
{
	if (str == "Example")
		return new Example();

	return NULL;
}


