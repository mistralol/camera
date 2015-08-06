
class System
{
	public:
		static int Reboot(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
		static int Shutdown(CameraHandler *handler, IServerConnection *Connection, Request *request, Request *response);
};


