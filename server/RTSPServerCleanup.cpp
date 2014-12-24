
#include <Camera.h>

RTSPServerCleanup::RTSPServerCleanup()
{

}

RTSPServerCleanup::~RTSPServerCleanup()
{
	m_mutex.Lock();
	m_exit = true;
	m_mutex.WakeUp();
	m_mutex.Unlock();
	Stop();
	g_object_unref(m_pool);
}

void RTSPServerCleanup::Init(GstRTSPServer *server)
{
	m_pool = gst_rtsp_server_get_session_pool(server);
	m_exit = false;
	Thread::Start();
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


