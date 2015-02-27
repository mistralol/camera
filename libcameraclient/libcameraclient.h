
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

		int VideoStreamCount(int *value);

		int VideoStreamSetEnabled(unsigned int stream, bool enabled);
		int VideoStreamGetEnabled(unsigned int stream, int *enabled);


		int UserCreate(const std::string Username, const std::string Password);
		int UserAuth(const std::string Username, const std::string Password);
		int UserDelete(const std::string Username);
		int UserExists(const std::string Username);
		int UserSetPassword(const std::string Username, const std::string Password);
		int UserList(std::list<std::string> *lst);

		int GroupCreate(const std::string Group);
		int GroupDelete(const std::string Group);
		int GroupExists(const std::string Group);
		int GroupIsUserInGroup(const std::string Group, const std::string User, int *result);
		int GroupUserAdd(const std::string Group, const std::string User);
		int GroupUserRemove(const std::string Group, const std::string User);
		int GroupList(std::list<std::string> *lst);
		int GroupListUsers(const std::string Group, std::list<std::string> *lst);

		int Version(std::string *str);
		std::string Version();

		int Ping();
		int Quit();

	private:
		ClientBase *m_Client;

};


