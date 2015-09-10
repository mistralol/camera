
#include <Camera.h>

Mutex User::m_mutex;
std::map<std::string, struct UserItem *> User::m_map;

bool User::Init()
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("User::Init()");
	if (m_map.empty() == false)
		abort();
	Create("admin", "admin", "");
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
	LogDebug("User::Destroy()");
	Reset();
}

bool User::ConfigLoad(Json::Value &json)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("User::ConfigLoad()");
	Reset();
	std::vector<std::string> lst = json.getMemberNames();
	std::vector<std::string>::iterator it = lst.begin();
	while(it != lst.end())
	{
		struct UserItem *item = new UserItem();
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
	LogDebug("User::ConfigSave()");
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

int User::Create(const std::string User, const std::string Password, const std::string EMail)
{
	const std::string passchar = "!\"£$%^&*()_+=#~{}[],./\?:;";
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("User::Create('%s', '', '%s')", User.c_str(), EMail.c_str());
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

	if (String::Sanity(&EMail, &passchar) == false)
	{
		LogError("User::Create - EMail contains invalid chars for user '%s'", User.c_str());
		return -EINVAL;
	}

	struct UserItem *item = new UserItem();
	struct timespec Now;
	Uuid uuid;
	Time::UTCNow(&Now);
	item->Key = uuid.ToString();
	item->Username = User;
	item->Password = Password;
	item->Created = Now.tv_sec;
	item->LastActivityDate = Now.tv_sec;
	item->LastLockoutDate = 0;
	item->LastLoginDate = 0;
	item->LastPasswordChange = Now.tv_sec;
	item->IsApproved = true;
	item->IsLockedOut = false;
	item->IsOnline = false;
	item->FailedPasswordAttempts = 0;
	m_map[User] = item;
	LogInfo("User::Create - Created user '%s'", User.c_str());
	return 0;
}

bool User::Auth(const std::string User, const std::string Password)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("User::Auth('%s')", User.c_str());
	struct timespec now;
	Time::UTCNow(&now);
	std::map<std::string, struct UserItem *>::iterator it = m_map.find(User);
	if (it == m_map.end())
	{
		LogError("User::Auth - No such user '%s'", User.c_str());
		return false;
	}
	struct UserItem *user = it->second;
	if (user->IsApproved == false)
	{
		LogInfo("User::Auth - Auth denied for user '%s' because account not approved", User.c_str());
		return false;
	}
	if (user->IsLockedOut == true)
	{
		LogInfo("User::Auth - Auth denied for user '%s' because account is locked out", User.c_str());
		return false;
	}
	
	if (user->Password == Password)
	{
		if (user->FailedPasswordAttempts > 0)
		{
			LogInfo("User::Auth - Granted for user '%s' after %d attempts", User.c_str(), user->FailedPasswordAttempts);
			user->FailedPasswordAttempts = 0;
		}
		else
		{
			LogInfo("User::Auth - Auth Granted for user '%s'", User.c_str());
		}
		user->LastActivityDate = now.tv_sec;
		user->LastLoginDate = now.tv_sec;
		user->IsOnline = true;
		return true;
	}

	if (user->FailedPasswordAttempts >= 10)
	{
		if (User == "admin")
		{
			LogWarning("User::Auth - Not locking out user '%s'", User.c_str());
			return false;
		}
		user->IsLockedOut = true;
		user->LastLockoutDate = now.tv_sec;
		LogWarning("User::Auth - Locked out user '%s' after %d failed attempts", User.c_str(), user->FailedPasswordAttempts);
		return false;
	}

	user->FailedPasswordAttempts++;
	LogInfo("User::Auth - Incorrect password for user '%s' Attempt = %d", User.c_str(), user->FailedPasswordAttempts);		
	return false;
}

int User::Delete(const std::string User)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("User::Auth('%s')", User.c_str());
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
	LogDebug("User::Auth('%s')", User.c_str());
	std::map<std::string, struct UserItem *>::iterator it = m_map.find(User);
	if (it == m_map.end())
		return 0;
	return 1;
}

int User::SetPassword(const std::string User, const std::string Password)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("User::SetPassword('%s')", User.c_str());
	struct timespec now;
	Time::UTCNow(&now);
	std::map<std::string, struct UserItem *>::iterator it = m_map.find(User);
	if (it == m_map.end())
	{
		LogError("User::SetPassword - No such user '%s'", User.c_str());
		return -ENOLINK;
	}
	it->second->Password = Password;
	it->second->LastPasswordChange = now.tv_sec;
	return 0;
}

int User::Touch(const std::string User)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("User::Touch('%s')", User.c_str());
	struct timespec now;
	Time::UTCNow(&now);
	std::map<std::string, struct UserItem *>::iterator it = m_map.find(User);
	if (it == m_map.end())
	{
		LogError("User::Touch - No such user '%s'", User.c_str());
		return -ENOLINK;
	}
	it->second->LastActivityDate = now.tv_sec;
	it->second->IsOnline = true;
	return 0;
}

int User::IsLockedOut(const std::string User)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("User::IsLockedOut('%s')", User.c_str());
	struct timespec now;
	Time::UTCNow(&now);
	std::map<std::string, struct UserItem *>::iterator it = m_map.find(User);
	if (it == m_map.end())
	{
		LogError("User::IsLockedOut - No such user '%s'", User.c_str());
		return -ENOLINK;
	}
	if (it->second->IsLockedOut)
		return 1;
	return 0;
}

int User::IsApproved(const std::string User)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("User::IsApproved('%s')", User.c_str());
	struct timespec now;
	Time::UTCNow(&now);
	std::map<std::string, struct UserItem *>::iterator it = m_map.find(User);
	if (it == m_map.end())
	{
		LogError("User::IsApproved - No such user '%s'", User.c_str());
		return -ENOLINK;
	}
	if (it->second->IsApproved)
		return 1;
	return 0;
}

int User::IsOnline(const std::string User)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("User::IsOnline('%s')", User.c_str());
	struct timespec now;
	Time::UTCNow(&now);
	std::map<std::string, struct UserItem *>::iterator it = m_map.find(User);
	if (it == m_map.end())
	{
		LogError("User::IsOnline - No such user '%s'", User.c_str());
		return -ENOLINK;
	}
	if (it->second->IsOnline)
		return 1;
	return 0;
}
		
int User::SetLockedOut(const std::string User, bool value)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("User::SetLockedOut('%s', %s)", User.c_str(), value ? "true" : "false");
	struct timespec now;
	Time::UTCNow(&now);
	std::map<std::string, struct UserItem *>::iterator it = m_map.find(User);
	if (it == m_map.end())
	{
		LogError("User::SetLockedOut - No such user '%s'", User.c_str());
		return -ENOLINK;
	}
	it->second->IsLockedOut = value;
	return 0;
}

int User::SetApproved(const std::string User, bool value)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("User::SetApproved('%s', %s)", User.c_str(), value ? "true" : "false");
	struct timespec now;
	Time::UTCNow(&now);
	std::map<std::string, struct UserItem *>::iterator it = m_map.find(User);
	if (it == m_map.end())
	{
		LogError("User::SetApproved - No such user '%s'", User.c_str());
		return -ENOLINK;
	}
	it->second->IsLockedOut = value;
	return 0;
}


int User::UserInfo(const std::string User, struct UserItem *item)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("User::UserInfo('%s')", User.c_str());
	std::map<std::string, struct UserItem *>::iterator it = m_map.find(User);
	if (it == m_map.end())
	{
		LogError("User::UserInfo - No such user '%s'", User.c_str());
		return -ENOLINK;
	}
	*item = *it->second;
	return 0;
}

std::vector<std::string> User::List()
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("User::List()");
	std::vector<std::string> lst;
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
	LogDebug("User::Lock()");
}

void User::Unlock()
{
	LogDebug("User::Unlock()");
	m_mutex.Unlock();
}

void User::Reset()
{
	LogDebug("User::Reset()");
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


