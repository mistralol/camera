
#include <main.h>

RTSPServerCleanup::RTSPServerCleanup()
{

}

RTSPServerCleanup::~RTSPServerCleanup()
{
	m_exit = true;
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
	while(m_exit == false)
	{
		gst_rtsp_session_pool_cleanup(m_pool);
		sleep(2);
	}
}


