
class CameraHandler;

class GPIOOutputTimer : public ITimer
{
	public:
		GPIOOutputTimer(unsigned int output, CameraHandler *handler, const struct timespec *ts, bool m_enabled);
		~GPIOOutputTimer();
		
		void GetDelay(struct timespec *ts);
		void TimerExpired(Timers *timers, ITimer *timer);
		

	private:
		unsigned int m_output;
		CameraHandler *m_handler;
		struct timespec m_timeout;
		bool m_enabled;
		
};

