
#include <cameractl.h>

static struct Operations Ops[] = {
	{"help", User::Help, "Shows this help"},
	{"create", User::Create, "Create a new user"},
	{"auth", User::Auth, "Autenticate a user"},
	{"delete", User::Delete, "delete a user"},
	{"exists", User::Exists, "check a user eists"},
	{"touch", User::Touch, "touch a user which marks then online"},
	{"lockedout", User::LockedOut, "get / set the lockout state of a user"},
	{"approved", User::Approved, "get / set the apporved state of a user"},
	{"online", User::Online, "check if a user is online"},
	{"password", User::Password, "set a password for a user"},
	{"info", User::Info, "dump all information for a user"},
	{"list", User::List, "Shows a list of users"},
	{"userfromkey", User::UserFromKey, "Get username from key"},
	{"userfromemail", User::UserFromEMail, "Get username from email"},
	{"lockoutduration", User::LockoutDuration, "Get / Set lockout duration"},
	{"maxfailedattempts", User::MaxFailedAttempts, "Get / Set max failed attempts"},
	{"autologoff", User::AutoLogOff, "Get / Set Auto logoff timeout"},
	{NULL, NULL, NULL }
};

void User::Create(struct Data *data)
{
	if (data->args.size() < 2 || data->args.size() > 3)
	{
		fprintf(stderr, "Invalid number of paramaters\n");
		return;
	}
	std::string username = data->args.front();
	data->args.pop_front();
	std::string password = data->args.front();
	data->args.pop_front();
	std::string email = "";
	if (data->args.size() > 0)
	{
		email = data->args.front();
		data->args.pop_front();
	}
	data->cli->UserCreate(username, password, email);
}

void User::Auth(struct Data *data)
{
	if (data->args.size() != 2)
	{
		fprintf(stderr, "Error need username and password as paramaters\n");
		return;
	}
	
	std::string username = data->args.front();
	data->args.pop_front();
	std::string password = data->args.front();
	data->args.pop_front();
	bool value = data->cli->UserAuth(username, password);
	printf("value: %d\n", value);
}

void User::Delete(struct Data *data)
{
	if (data->args.size() != 1)
	{
		fprintf(stderr, "Error need username as paramater\n");
		return;
	}
	
	std::string username = data->args.front();
	data->args.pop_front();
	data->cli->UserDelete(username);
}

void User::Exists(struct Data *data)
{
	if (data->args.size() != 1)
	{
		fprintf(stderr, "Error need username as paramater\n");
		return;
	}
	
	std::string username = data->args.front();
	data->args.pop_front();
	bool value = data->cli->UserExists(username);
	printf("value: %d\n", value);
}

void User::Touch(struct Data *data)
{
	if (data->args.size() != 1)
	{
		fprintf(stderr, "Error need username as paramater\n");
		return;
	}
	
	std::string username = data->args.front();
	data->args.pop_front();
	data->cli->UserTouch(username);
}

void User::LockedOut(struct Data *data)
{
	if (data->args.size() == 1)
	{
		std::string username = data->args.front();
		data->args.pop_front();
		bool locked = data->cli->UserIsLockedOut(username);
		printf("locked: %d\n", locked);
		return;
	}

	if (data->args.size() == 2)
	{
		std::string username = data->args.front();
		data->args.pop_front();
		std::string str = data->args.front();
		data->args.pop_front();
		int value = atoi(str.c_str());
		data->cli->UserSetLockedOut(username, value ? true : false);
	}

	fprintf(stderr, "Invalid number of paramaters\n");
	return;
}

void User::Approved(struct Data *data)
{
	if (data->args.size() == 1)
	{
		std::string username = data->args.front();
		data->args.pop_front();
		bool value = data->cli->UserIsApproved(username);
		printf("value: %d\n", value);
		return;
	}

	if (data->args.size() == 2)
	{
		std::string username = data->args.front();
		data->args.pop_front();
		std::string str = data->args.front();
		data->args.pop_front();
		int value = atoi(str.c_str());
		data->cli->UserSetApproved(username, value ? true : false);
	}

	fprintf(stderr, "Invalid number of paramaters\n");
	return;
}

void User::Online(struct Data *data)
{
	if (data->args.size() != 1)
	{
		fprintf(stderr, "Error need username as paramater\n");
		return;
	}
	
	std::string username = data->args.front();
	data->args.pop_front();
	bool value = data->cli->UserIsOnline(username);
	printf("online: %d\n", value);
}

void User::Password(struct Data *data)
{
	if (data->args.size() != 2)
	{
		fprintf(stderr, "Error need username and password as paramaters\n");
		return;
	}
	
	std::string username = data->args.front();
	data->args.pop_front();
	std::string password = data->args.front();
	data->args.pop_front();
	data->cli->UserSetPassword(username, password);
}

void User::Info(struct Data *data)
{
	if (data->args.size() != 1)
	{
		fprintf(stderr, "Error need username as paramater\n");
		return;
	}
	
	std::string username = data->args.front();
	data->args.pop_front();
	UserItem info;
	data->cli->UserInfo(username, &info);
	
	printf("Key: %s\n", info.Key.c_str());
	printf("Username: %s\n", info.Username.c_str());
	printf("Password: %s\n", info.Password.c_str());
	printf("EMail: %s\n", info.EMail.c_str());
	
	printf("Created: %s", ctime(&info.Created));
	printf("LastActivityDate: %s", ctime(&info.LastActivityDate));
	printf("LastLockoutDate: %s", ctime(&info.LastLockoutDate));
	printf("LastLoginDate: %s", ctime(&info.LastLoginDate));
	printf("LastPasswordChange: %s", ctime(&info.LastPasswordChange));

	printf("IsApproved: %s\n", info.IsApproved ? "true" : "false");
	printf("IsLockedOut: %s\n", info.IsLockedOut ? "true" : "false");
	printf("IsOnline: %s\n", info.IsOnline ? "true" : "false");
	printf("FailedPasswordAttempts: %d\n", info.FailedPasswordAttempts);
	
}

void User::UserFromEMail(struct Data *data)
{
	if (data->args.size() != 1)
	{
		fprintf(stderr, "Error need key as paramater\n");
		return;
	}
	
	std::string username;
	std::string email = data->args.front();
	data->args.pop_front();
	username = data->cli->UserGetUserFromEMail(email);
	printf("%s\n", username.c_str());
}

void User::UserFromKey(struct Data *data)
{
	if (data->args.size() != 1)
	{
		fprintf(stderr, "Error need key as paramater\n");
		return;
	}
	
	std::string username;
	std::string key = data->args.front();
	data->args.pop_front();
	username = data->cli->UserGetUserFromKey(key);
	printf("%s\n", username.c_str());
}

void User::LockoutDuration(struct Data *data)
{
	if (data->args.size() == 0)
	{
		int ret = data->cli->UserGetLockoutDuration();
		printf("value: %d\n", ret);
		return;
	}

	if (data->args.size() == 1)
	{
		std::string str = data->args.front();
		data->args.pop_front();
		int value = atoi(str.c_str());
		data->cli->UserSetLockoutDuration(value);
		return;
	}

	fprintf(stderr, "Invalid number of paramaters\n");
}

void User::MaxFailedAttempts(struct Data *data)
{
	if (data->args.size() == 0)
	{
		int ret = data->cli->UserGetMaxFailedAttempts();
		printf("Max: %d\n", ret);
	}

	if (data->args.size() == 1)
	{
		std::string str = data->args.front();
		data->args.pop_front();
		int value = atoi(str.c_str());
		data->cli->UserSetMaxFailedAttempts(value);
		return;
	}

	fprintf(stderr, "Invalid number of paramaters\n");
}

void User::AutoLogOff(struct Data *data)
{
	if (data->args.size() == 0)
	{
		int ret = data->cli->UserGetAutoLogOff();
		printf("Enabled: %d\n", ret);
	}

	if (data->args.size() == 1)
	{
		std::string str = data->args.front();
		data->args.pop_front();
		int value = atoi(str.c_str());
		data->cli->UserSetAutoLogOff(value);
		return;
	}

	fprintf(stderr, "Invalid number of paramaters\n");
}

void User::List(struct Data *data)
{
	std::vector<std::string> lst = data->cli->UserList();
	for(std::vector<std::string>::iterator it = lst.begin(); it != lst.end(); it++)
	{
		printf("%s\n", it->c_str());
	}
}

void User::Help(struct Data *data)
{
	DumpHelp(Ops);
}


void User::Process(struct Data *data)
{
	data->ops = Ops;
	::Process(data);
}
