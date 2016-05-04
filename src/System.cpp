
#include <Camera.h>

int System::Reboot(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	LogInfo("System::Reboot");
	UserID user = UserID();

	if (user.Up(0) < 0)
	{
		LogError("System::Reboot - Cannot set uid(0)");
		return -EPERM;
	}

	if (system("/sbin/reboot") != 0)
	{
		LogError("System::Reboot - /sbin/reboot failed");
		return -1;
	}

	return 0;
}

int System::Shutdown(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response)
{
	LogInfo("System::Shutdown");
	UserID user = UserID();

	if (user.Up(0) < 0)
	{
		LogError("System::Shutdown - Cannot set uid(0)");
		return -EPERM;
	}

	if (system("/sbin/reboot") != 0)
	{
		LogError("System::Shutdown - /sbin/reboot failed");
		return -1;
	}

	return 0;
}

