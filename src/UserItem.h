
class UserItem
{
	public:
		UserItem();
		~UserItem();
		
		bool ConfigLoad(Json::Value &json);
		bool ConfigSave(Json::Value &json);
		
		std::string Key;
		std::string Username;
		std::string Password;
		std::string EMail;

		time_t Created;
		time_t LastActivityDate;
		time_t LastLockoutDate;
		time_t LastLoginDate;
		time_t LastPasswordChange;

		bool IsApproved;
		bool IsLockedOut;
		bool IsOnline;
	
		int FailedPasswordAttempts;

};

