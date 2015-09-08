

class User
{
	public:
		static int List(struct Data *data);
		static int Create(struct Data *data);
		static int Auth(struct Data *data);
		static int Delete(struct Data *data);
		static int Exists(struct Data *data);
		static int Touch(struct Data *data);
		static int LockedOut(struct Data *data);
		static int Approved(struct Data *data);
		static int Online(struct Data *data);
		static int Password(struct Data *data);
		static int Info(struct Data *data);
		
		static int Help(struct Data *data);
		static int Process(struct Data *data);
};

