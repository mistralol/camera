
#include <Camera.h>

RTSPServerCleanup::RTSPServerCleanup()
{
	m_exit = false;
	m_started = false;
	m_pool = NULL;
}

RTSPServerCleanup::~RTSPServerCleanup()
{
	if (m_started)
	{
		Stop();
	}
}

void RTSPServerCleanup::Start(GstRTSPServer *server)
{
	m_pool = gst_rtsp_server_get_session_pool(server);
	m_exit = false;
	m_started = true;
	Thread::Start();
}

void RTSPServerCleanup::Stop()
{
		m_mutex.Lock();
		m_exit = true;
		m_started = false;
		m_mutex.WakeUp();
		m_mutex.Unlock();
		Thread::Stop();
		g_object_unref(m_pool);
}

void RTSPServerCleanup::Run()
{
	struct timespec timeout = {60, 0}; //60 Seconds Wait
	m_mutex.Lock();
	while(m_exit == false)
	{
		m_mutex.Wait(&timeout);
		m_mutex.Unlock();
		LogDebug("RTSPServerCleanup::Run() SessionCount: %d", gst_rtsp_session_pool_get_n_sessions(m_pool));
		gst_rtsp_session_pool_cleanup(m_pool);
		m_mutex.Lock();
	}
	m_mutex.Unlock();
}


