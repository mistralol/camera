
#include <main.h>

RTSPServer::RTSPServer()
{
	m_loop = NULL;
	m_server = NULL;
	Thread::Start();
	m_startbar.Wait();
}

RTSPServer::~RTSPServer()
{
	g_main_loop_quit(m_loop);
	Thread::Stop();
}

void RTSPServer::PipelineAdd(const std::string url, const std::string pipeline)
{
	GstRTSPMediaMapping *mapping = gst_rtsp_server_get_media_mapping (m_server);
	GstRTSPMediaFactory *factory = gst_rtsp_media_factory_new ();

	gst_rtsp_media_factory_set_launch (factory, pipeline.c_str());
	gst_rtsp_media_factory_set_shared (factory, TRUE);
	gst_rtsp_media_mapping_add_factory (mapping, url.c_str(), factory);

	g_object_unref(mapping);
}

void RTSPServer::PipelineRemove(const std::string url)
{
	GstRTSPMediaMapping *mapping = gst_rtsp_server_get_media_mapping (m_server);
	gst_rtsp_media_mapping_remove_factory(mapping, url.c_str());
	g_object_unref (mapping);	
}

void RTSPServer::SessionsSetMax(guint max)
{
	GstRTSPSessionPool *Pool = gst_rtsp_server_get_session_pool(m_server);
	gst_rtsp_session_pool_set_max_sessions(Pool, max);
	g_object_unref(Pool);
}

guint RTSPServer::SessionsGetMax()
{
	guint max = 0;
	GstRTSPSessionPool *Pool = gst_rtsp_server_get_session_pool(m_server);
	max = gst_rtsp_session_pool_get_max_sessions(Pool);
	g_object_unref(Pool);
	return max;
}

guint RTSPServer::SessionsCount()
{
	guint current = 0;
	GstRTSPSessionPool *Pool = gst_rtsp_server_get_session_pool(m_server);
	current = gst_rtsp_session_pool_get_n_sessions(Pool);
	g_object_unref(Pool);
	return current;
}

void RTSPServer::BacklogSet(guint max)
{
	gst_rtsp_server_set_backlog(m_server, max);
}

guint RTSPServer::BacklogGet(guint max)
{
	return gst_rtsp_server_get_backlog(m_server);
}

void RTSPServer::Run()
{
	RTSPServerCleanup PoolCleaner;

	m_loop = g_main_loop_new (NULL, FALSE);
	m_server = gst_rtsp_server_new ();

	gst_rtsp_server_attach (m_server, NULL);

	PoolCleaner.Init(m_server);

	m_startbar.WakeUp();
	g_main_loop_run(m_loop);

	g_object_unref(m_server);
	g_main_loop_unref(m_loop);
}

