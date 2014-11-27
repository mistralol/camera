
#include <string>

#include <liblogger.h>

using namespace liblogger;

#include "../PlatformBase.h"
#include "Example.h"

Example::Example()
{

}

Example::~Example()
{

}

bool Example::Init()
{
	LogDebug("Example::Init");

	return true;
}

