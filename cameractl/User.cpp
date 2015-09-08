
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
	{NULL, NULL, NULL }
};

int User::Create(struct Data *data)
{
	if (data->args.size() < 2 || data->args.size() > 3)
	{
		printf("Invalid number of paramaters\n");
		return -1;
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
	return data->cli->UserCreate(username, password, email);
}

int User::Auth(struct Data *data)
{
	if (data->args.size() != 2)
	{
		printf("Error need username and password as paramaters\n");
		return -1;
	}
	
	std::string username = data->args.front();
	data->args.pop_front();
	std::string password = data->args.front();
	data->args.pop_front();
	return data->cli->UserAuth(username, password);
}

int User::Delete(struct Data *data)
{
	if (data->args.size() != 1)
	{
		printf("Error need username as paramater\n");
		return -1;
	}
	
	std::string username = data->args.front();
	data->args.pop_front();
	return data->cli->UserDelete(username);
}

int User::Exists(struct Data *data)
{
	if (data->args.size() != 1)
	{
		printf("Error need username as paramater\n");
		return -1;
	}
	
	std::string username = data->args.front();
	data->args.pop_front();
	return data->cli->UserExists(username);
}

int User::Touch(struct Data *data)
{
	if (data->args.size() != 1)
	{
		printf("Error need username as paramater\n");
		return -1;
	}
	
	std::string username = data->args.front();
	data->args.pop_front();
	return data->cli->UserTouch(username);
}

int User::LockedOut(struct Data *data)
{
	if (data->args.size() == 1)
	{
		std::string username = data->args.front();
		data->args.pop_front();
		return data->cli->UserIsLockedOut(username);
	}

	if (data->args.size() == 2)
	{
		std::string username = data->args.front();
		data->args.pop_front();
		std::string str = data->args.front();
		data->args.pop_front();
		int value = atoi(str.c_str());
		return data->cli->UserSetLockedOut(username, value ? true : false);
	}

	printf("Invalid number of paramaters\n");
	return -1;
}

int User::Approved(struct Data *data)
{
	if (data->args.size() == 1)
	{
		std::string username = data->args.front();
		data->args.pop_front();
		return data->cli->UserIsApproved(username);
	}

	if (data->args.size() == 2)
	{
		std::string username = data->args.front();
		data->args.pop_front();
		std::string str = data->args.front();
		data->args.pop_front();
		int value = atoi(str.c_str());
		return data->cli->UserSetApproved(username, value ? true : false);
	}

	printf("Invalid number of paramaters\n");
	return -1;
}

int User::Online(struct Data *data)
{
	if (data->args.size() != 1)
	{
		printf("Error need username as paramater\n");
		return -1;
	}
	
	std::string username = data->args.front();
	data->args.pop_front();
	return data->cli->UserIsOnline(username);
}

int User::Password(struct Data *data)
{
	if (data->args.size() != 2)
	{
		printf("Error need username and password as paramaters\n");
		return -1;
	}
	
	std::string username = data->args.front();
	data->args.pop_front();
	std::string password = data->args.front();
	data->args.pop_front();
	return data->cli->UserSetPassword(username, password);
}

int User::Info(struct Data *data)
{
	if (data->args.size() != 1)
	{
		printf("Error need username as paramater\n");
		return -1;
	}
	
	std::string username = data->args.front();
	data->args.pop_front();
	struct UserItem info;
	int ret = data->cli->UserInfo(username, &info);
	if (ret < 0)
		return ret;
	
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
	
	return ret;
}

int User::List(struct Data *data)
{
	std::vector<std::string> lst;
	int ret = data->cli->UserList(lst);
	if (ret < 0)
		return ret;
	for(std::vector<std::string>::iterator it = lst.begin(); it != lst.end(); it++)
	{
		printf("%s\n", it->c_str());
	}
	return ret;
}

int User::Help(struct Data *data)
{
	return DumpHelp(Ops);
}


int User::Process(struct Data *data)
{
	data->ops = Ops;
	return ::Process(data);
}
