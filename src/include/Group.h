
class Group
{
	public:
		static bool ConfigLoad(Json::Value &json);
		static bool ConfigSave(Json::Value &json);

		static int Create(const std::string Group);
		static int Delete(const std::string Group);
		static int Exists(const std::string Group);
		static int IsUserInGroup(const std::string Group, const std::string User);

		static int UserAdd(const std::string Group, const std::string User);
		static int UserRemove(const std::string Group, const std::string User);
		static void UserRemoveFromAll(const std::string User);

		static std::list<std::string> List();
		static std::list<std::string> ListUsers(const std::string Group);

	private:
		static Mutex m_mutex;
		static std::map<std::string, std::list<std::string> > m_map;

};

