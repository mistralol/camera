
class UserScopedLock
{
	public:
		UserScopedLock();
		~UserScopedLock();

		void Unlock();

	private:
		bool m_locked;

};

