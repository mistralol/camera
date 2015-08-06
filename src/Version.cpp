
#include <Camera.h>

#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_REVISION 0
#define VERSION_BUILD 1

unsigned int Version::Major()
{
	return VERSION_MAJOR;
}

unsigned int Version::Minor()
{
	return VERSION_MINOR;
}

unsigned int Version::Revision()
{
	return VERSION_REVISION;
}

unsigned int Version::Build()
{
	return VERSION_BUILD;
}

std::string Version::ToString()
{
	std::stringstream ss;
	ss << Major() << "." << Minor() << "." << Revision() << "." << Build();
	return ss.str();
}

