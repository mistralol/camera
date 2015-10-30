
#include <Camera.h>

GPIOOutputTimer::GPIOOutputTimer(unsigned int output, CameraHandler *handler, const struct timespec *timeout, bool enabled) :
	m_output(output),
	m_handler(handler),
	m_timeout(*timeout),
	m_enabled(enabled)
{

}

GPIOOutputTimer::~GPIOOutputTimer()
{

}
		
time_t GPIOOutputTimer::GetDelay()
{
	return m_timeout.tv_sec;	
}

void GPIOOutputTimer::TimerExpired(Timers *timers, ITimer *timer)
{
	m_handler->GPIOOutputSetState(m_output, m_enabled);
}

