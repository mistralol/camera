
#include <Camera.h>

UserScopedLock::UserScopedLock() :
	m_locked(false)
{
	User::Lock();
	m_locked = true;
}

UserScopedLock::~UserScopedLock()
{
	m_locked = false;
	User::Unlock();
}

void UserScopedLock::Unlock()
{
	if (m_locked == false)
		abort(); //Somebody has been stupid
	m_locked = false;
	User::Unlock();
}

