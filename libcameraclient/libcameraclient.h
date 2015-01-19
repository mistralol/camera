
class CameraClient
{
	public:
		CameraClient();
		~CameraClient();

		bool Connect(const std::string ConnectionString);
		void Disconnect();

		int RTSPGetClientCount(int *value);

		int RTSPSetMaxClients(int max);
		int RTSPGetMaxClients(int *value);

		int RTSPSetMaxBacklog(int max);
		int RTSPGetMaxBacklog(int *value);

		int Version(std::string *str);
		std::string Version();

		int Ping();
		int Quit();

	private:
		ClientBase *m_Client;

};


