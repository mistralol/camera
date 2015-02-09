
class CameraClient
{
	public:
		CameraClient();
		~CameraClient();

		bool Connect(const std::string ConnectionString);
		void Disconnect();

		int RTSPGetPort(int *value);
		int RTSPSetPort(int value);

		int RTSPGetClientCount(int *value);

		int RTSPSetMaxClients(int max);
		int RTSPGetMaxClients(int *value);

		int RTSPSetMaxBacklog(int max);
		int RTSPGetMaxBacklog(int *value);

		int VideoStreamSetEnabled(unsigned int stream, bool enabled);
		int VideoStreamGetEnabled(unsigned int stream, int *enabled);

		int Version(std::string *str);
		std::string Version();

		int Ping();
		int Quit();

	private:
		ClientBase *m_Client;

};


