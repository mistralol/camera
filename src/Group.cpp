
#include <Camera.h>

Mutex Group::m_mutex;
std::map<std::string, std::list<std::string> > Group::m_map;

bool Group::ConfigLoad(Json::Value &json)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("Group::ConfigLoad");
	m_map.clear();
	std::vector<std::string> lst = json.getMemberNames();
	std::vector<std::string>::iterator it = lst.begin();
	while(it != lst.end())
	{
		if (m_map.find(*it) == m_map.end())
			m_map[*it] = std::list<std::string>();

		std::vector<std::string> lst2 = json.getMemberNames();
		std::vector<std::string>::iterator it2 = lst2.begin();
		while(it2 != lst2.end())
		{
			m_map[*it].push_back(*it2);
			it2++;
		}
		it++;
	}

	return true;
}

bool Group::ConfigSave(Json::Value &json)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("Group::ConfigSave");
	std::map<std::string, std::list<std::string> >::iterator it = m_map.begin();
	while(it != m_map.end())
	{
		json[it->first];
		std::list<std::string>::iterator uit = it->second.begin();
		while(uit != it->second.end())
		{
			json[it->first] = *uit;
			uit++;
		}
		it++;
	}
	return true;
}

int Group::Create(const std::string Group)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("Group::Create('%s')", Group.c_str());
	if (Exists(Group))
	{
		LogError("Group::Create - Cannot add group '%s' because group already exists", Group.c_str());
		return -EEXIST;
	}

	if (String::Sanity(&Group) == false)
	{
		LogError("Group::Create - Group '%s' contains invalid chars", Group.c_str());
		return -EINVAL;
	}

	m_map[Group] = std::list<std::string>();
	LogInfo("Group::Create - Added group '%s'", Group.c_str());
	return 0;
}

int Group::Delete(const std::string Group)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("Group::Delete('%s')", Group.c_str());
	if (Group == "Admin")
	{
		LogCritical("Group::Delete - Rejecting attempt to delete group '%s'", Group.c_str());
		return -EPERM;
	}
	std::map<std::string, std::list<std::string> >::iterator it = m_map.begin();
	while(it != m_map.end())
	{
		if (it->first == Group)
		{
			m_map.erase(it);
			LogInfo("Group::Delete - Deleted group '%s'", Group.c_str());
			return 0;
		}
		it++;
	}
	LogError("Group::Delete - Group '%s' not found", Group.c_str());
	return -ENOLINK;
}

int Group::Exists(const std::string Group)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("Group::Exists('%s')", Group.c_str());
	std::map<std::string, std::list<std::string> >::iterator it = m_map.begin();
	while(it != m_map.end())
	{
		if (it->first == Group)
			return 1;
		it++;
	}
	return 0;
}

int Group::IsUserInGroup(const std::string Group, const std::string User)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("Group::IsUserInGroup('%s', '%s')", Group.c_str(), User.c_str());
	std::map<std::string, std::list<std::string> >::iterator it = m_map.begin();
	while(it != m_map.end())
	{
		if (it->first == Group)
		{
			std::list<std::string>::iterator uit = it->second.begin();
			while(uit != it->second.end())
			{
				if (*uit == User)
				{
					return 1;
				}
				uit++;
			}
			return 0;
		}
		it++;
	}
	return 0;
}

int Group::UserAdd(const std::string Group, const std::string User)
{
	User::Lock(); //Don't allow the user to be deleted while we are adding it to a group
	if (User::Exists(User) == false)
	{
		LogError("Group::UserAdd - User '%s' does not exist\n", User.c_str());
		User::Unlock();
		return -EINVAL;
	}
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("Group::UserAdd('%s', '%s')", Group.c_str(), User.c_str());
	std::map<std::string, std::list<std::string> >::iterator it = m_map.begin();
	while(it != m_map.end())
	{
		if (it->first == Group)
		{
			std::list<std::string>::iterator uit = it->second.begin();
			while(uit != it->second.end())
			{
				if (*uit == User)
				{
					LogError("Group::UserAdd - Cannot add user '%s' to group '%s' because already exists", User.c_str(), Group.c_str());
					User::Unlock();
					return -EEXIST;
				}
				uit++;
			}
			it->second.push_back(User);
			User::Unlock();
			return 0;
		}
		it++;
	}
	LogError("Group::UserAdd - Cannot add user '%s' to group '%s' because group does not exist", User.c_str(), Group.c_str());
	User::Unlock();
	return -ENOLINK;
}

int Group::UserRemove(const std::string Group, const std::string User)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("Group::UserRemove('%s', '%s')", Group.c_str(), User.c_str());
	std::map<std::string, std::list<std::string> >::iterator it = m_map.begin();
	if (Group == "admin" && User == "admin")
	{
		LogCritical("Group::UserRemove - Rejected attempt to remove user '%s' from group '%s'", User.c_str(), Group.c_str());
		return -EPERM;
	}
	while(it != m_map.end())
	{
		if (it->first == Group)
		{
			std::list<std::string>::iterator uit = it->second.begin();
			while(uit != it->second.end())
			{
				if (*uit == User)
				{
					it->second.erase(uit);
					LogInfo("Group::UserRemove - Removed user '%s' from group '%s'", User.c_str(), Group.c_str());
					return 0;
				}
				uit++;
			}
			LogError("Group::UserRemove - Cannot remove user '%s' from group '%s' because user does not exist", User.c_str(), Group.c_str());
			return -ENOLINK;
		}
		it++;
	}
	LogError("Group::UserRemove - Cannot remove user '%s' from group '%s' because group does not exist", User.c_str(), Group.c_str());
	return -ENOLINK;
}

void Group::UserRemoveFromAll(const std::string User)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("Group::UserRemoveFromAll('%s')", User.c_str());
	std::map<std::string, std::list<std::string> >::iterator it = m_map.begin();
	while(it != m_map.end())
	{
		std::list<std::string>::iterator uit = it->second.begin();
		while(uit != it->second.end())
		{
			if (*uit == User)
			{
				it->second.erase(uit);
				LogInfo("Group::UserRemoveFromAll - Removed user '%s' from group '%s'", User.c_str(), it->first.c_str());
				break; //Should never be in the group move than once
			}
			uit++;
		}
		it++;
	}
}

std::list<std::string> Group::List()
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("Group::List()");
	std::list<std::string> lst;
	std::map<std::string, std::list<std::string> >::iterator it = m_map.begin();
	while(it != m_map.end())
	{
		lst.push_back(it->first);
		it++;
	}
	return lst;
}

std::list<std::string> Group::ListUsers(const std::string Group)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("Group::ListUsers('%s')", Group.c_str());
	std::list<std::string> lst;
	std::map<std::string, std::list<std::string> >::iterator it = m_map.begin();
	while(it != m_map.end())
	{
		if (it->first == Group)
			return it->second;
	}
	return lst; //Return empty list for if no group found
}

