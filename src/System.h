
class System
{
	public:
		static int Reboot(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
		static int Shutdown(CameraHandler *handler, IServerConnection *Connection, Json::Value &request, Json::Value &response);
};


