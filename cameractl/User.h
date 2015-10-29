

class User
{
	public:
		static void List(struct Data *data);
		static void Create(struct Data *data);
		static void Auth(struct Data *data);
		static void Delete(struct Data *data);
		static void Exists(struct Data *data);
		static void Touch(struct Data *data);
		static void LockedOut(struct Data *data);
		static void Approved(struct Data *data);
		static void Online(struct Data *data);
		static void Password(struct Data *data);
		static void Info(struct Data *data);

		static void UserFromEMail(struct Data *data);		
		static void UserFromKey(struct Data *data);
		static void LockoutDuration(struct Data *data);
		static void MaxFailedAttempts(struct Data *data);
		static void AutoLogOff(struct Data *data);
		
		static void Help(struct Data *data);
		static void Process(struct Data *data);
};

