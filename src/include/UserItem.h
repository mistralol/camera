
struct UserItem
{
	std::string Username;
	std::string Password;

	void Init()
	{
		Username = "";
		Password = "";
	}

	bool ConfigLoad(Json::Value &json)
	{
		if (json.isMember("username") == false)
			return false;
		Username = json["username"].asString();

		if (json.isMember("password") == false)
			return false;
		Password = json["password"].asString();

		return true;
	}

	bool ConfigSave(Json::Value &json)
	{
		json["username"] = Username;
		json["password"] = Password;
		return true;
	}
};

