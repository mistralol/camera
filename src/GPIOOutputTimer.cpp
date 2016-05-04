
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
		
void GPIOOutputTimer::GetDelay(struct timespec *ts)
{
	*ts = m_timeout;
}

void GPIOOutputTimer::TimerExpired(Timers *timers, ITimer *timer)
{
	//m_handler->GPIOOutputSetState(m_output, m_enabled);
}

