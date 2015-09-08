
class User
{
	public:
		static bool Init();
		static void Destroy();
		static bool ConfigLoad(Json::Value &json);
		static bool ConfigSave(Json::Value &json);

		static int Create(const std::string User, const std::string Password, const std::string EMail);
		static bool Auth(const std::string User, const std::string Password);
		static int Delete(const std::string User);
		static int Exists(const std::string User);
		static int SetPassword(const std::string User, const std::string Password);
		static int Touch(const std::string User);

		static int IsLockedOut(const std::string User);
		static int IsApproved(const std::string User);
		static int IsOnline(const std::string User);
		
		static int SetLockedOut(const std::string User, bool value);
		static int SetApproved(const std::string User, bool value);

		static int UserInfo(const std::string User, struct UserItem *);

		static std::list<std::string> List();

		static void Lock();
		static void Unlock();

	protected:
		static void Reset();


	private:
		static Mutex m_mutex;
		static std::map<std::string, struct UserItem *> m_map;

};

