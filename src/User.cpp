
#include <Camera.h>

Mutex User::m_mutex;
std::map<std::string, struct UserItem *> User::m_map;

bool User::Init()
{
	ScopedLock lock = ScopedLock(&m_mutex);
	if (m_map.empty() == false)
		abort();
	Create("admin", "admin");
	if (Group::Exists("admin") == false)
	{
		if (Group::Create("admin") < 0)
		{
			LogCritical("User::Init - Group::Create Failed");
			return false;
		}
	}
	if (Group::UserAdd("admin", "admin") < 0)
	{
		LogCritical("User::Init - GroupUserAdd Failed");
		return false;
	}
	return true;
}

void User::Destroy()
{
	ScopedLock lock = ScopedLock(&m_mutex);
	Reset();
}

bool User::ConfigLoad(Json::Value &json)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("User::ConfigLoad");
	Reset();
	std::vector<std::string> lst = json.getMemberNames();
	std::vector<std::string>::iterator it = lst.begin();
	while(it != lst.end())
	{
		struct UserItem *item = new UserItem();
		item->Init();
		if (item->ConfigLoad(json[*it]) == false)
		{
			delete item;
			return false;
		}
		m_map[*it] = item;
		it++;
	}
	return true;
}

bool User::ConfigSave(Json::Value &json)
{
	bool fail = false;
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("User::ConfigSave");
	std::map<std::string, struct UserItem *>::iterator it = m_map.begin();
	while(it != m_map.end())
	{
		fail = it->second->ConfigSave(json[it->first]);
		if (fail == false)
		{
			LogCritical("User::ConfigSave - Failed to save user '%s'", it->first.c_str());
			return false;
		}
		it++;
	}

	return true;
}

int User::Create(const std::string User, const std::string Password)
{
	const std::string passchar = "!\"£$%^&*()_+=#~{}[],./\?:;";
	ScopedLock lock = ScopedLock(&m_mutex);
	if (m_map.find(User) != m_map.end())
	{
		LogError("User::Create - Username '%s' already exists", User.c_str());
		return -EEXIST;
	}

	if (String::Sanity(&User) == false)
	{
		LogError("User::Create - Username '%s' contains invalid chars", User.c_str());
		return -EINVAL;
	}

	if (String::Sanity(&User, &passchar) == false)
	{
		LogError("User::Create - Password contains invalid chars for user '%s'", User.c_str());
		return -EINVAL;
	}

	struct UserItem *item = new UserItem();
	item->Username = User;
	item->Password = Password;
	m_map[User] = item;
	LogInfo("User::Create - Created user '%s'", User.c_str());
	return 0;
}

bool User::Auth(const std::string User, const std::string Password)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	std::map<std::string, struct UserItem *>::iterator it = m_map.find(User);
	if (it == m_map.end())
	{
		LogError("User::Auth - No such user '%s'", User.c_str());
		return false;
	}
	if (it->second->Password == Password)
		return true;
	return false;
}

int User::Delete(const std::string User)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	if (User == "admin")
	{
		LogError("User::Delete - Attempted to delete admin user");
		return -EPERM;
	}
	std::map<std::string, struct UserItem *>::iterator it = m_map.find(User);
	if (it == m_map.end())
	{
		LogError("User::Delete - Cannot find user '%s'", User.c_str());
		return -ENOLINK;
	}
	delete it->second;
	m_map.erase(it);
	Group::UserRemoveFromAll(User);
	LogInfo("User::Delete - Deleted User '%s'", User.c_str());
	return 0;
}

int User::Exists(const std::string User)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	std::map<std::string, struct UserItem *>::iterator it = m_map.find(User);
	if (it == m_map.end())
		return 0;
	return 1;
}

int User::SetPassword(const std::string User, const std::string Password)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	std::map<std::string, struct UserItem *>::iterator it = m_map.find(User);
	if (it == m_map.end())
	{
		LogError("User::SetPassword - No such user '%s'", User.c_str());
		return -ENOLINK;
	}
	it->second->Password = Password;
	return 0;
}

std::list<std::string> User::List()
{
	ScopedLock lock = ScopedLock(&m_mutex);
	std::list<std::string> lst;
	std::map<std::string, struct UserItem *>::iterator it = m_map.begin();
	while(it != m_map.end())
	{
		lst.push_back(it->first);
		it++;
	}
	return lst;
}

void User::Lock()
{
	m_mutex.Lock();
}

void User::Unlock()
{
	m_mutex.Unlock();
}

void User::Reset()
{
#ifdef DEBUG
	if (m_mutex.IsOwner == false)
		abort(); //Must be called locked
#endif
	std::map<std::string, struct UserItem *>::iterator it = m_map.begin();
	while(it != m_map.end())
	{
		delete it->second;
		it++;
	}
	m_map.clear();
}


