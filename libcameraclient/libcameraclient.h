
class CameraClient
{
	public:
		CameraClient();
		~CameraClient();

		bool Connect(const std::string ConnectionString);
		void Disconnect();

		int Ping();
		int Quit();

	private:
		ClientBase *m_Client;

};

