
#include <Camera.h>

RTSPServer::RTSPServer()
{
	LogDebug("RTSPServer::RTSPServer");
	m_loop = NULL;
	m_server = NULL;
	Thread::Start();
	m_startbar.Wait();
}

RTSPServer::~RTSPServer()
{
	LogDebug("RTSPServer::~RTSPServer");
	g_main_loop_quit(m_loop);
	Thread::Stop();
}

void RTSPServer::PipelineAdd(const std::string url, const std::string pipeline)
{
	LogDebug("RTSPServer::PipelineAdd(%s, %s)", url.c_str(), pipeline.c_str());

	GstRTSPMountPoints *mounts = gst_rtsp_server_get_mount_points (m_server);
	GstRTSPMediaFactory *factory = gst_rtsp_media_factory_new ();

	gst_rtsp_media_factory_set_launch (factory, pipeline.c_str());
	gst_rtsp_media_factory_set_shared (factory, TRUE);
	gst_rtsp_mount_points_add_factory (mounts, url.c_str(), factory);

	g_object_unref(mounts);
}

void RTSPServer::PipelineRemove(const std::string url)
{
	LogDebug("RTSPServer::PipelineRemove(%s)", url.c_str());

	GstRTSPMountPoints *mounts = gst_rtsp_server_get_mount_points (m_server);
	gst_rtsp_mount_points_remove_factory(mounts, url.c_str());
	g_object_unref (mounts);	
}

int RTSPServer::SetPort(int port)
{
	LogDebug("RTSPServer::SetPort(%d)", port);
	if (port < 0 || port > 65535)
	{
		LogError("RTSPServer::SetPort(%d) - Value out of range < 0 || > 65535", port);
		return -EINVAL;
	}
	std::stringstream ss;
	ss << port;
	gst_rtsp_server_set_service(m_server, ss.str().c_str());
	if (GetPort() != port)
	{
		LogError("RTSPServer::SetPort(%d) Failed Current Port is %d", port, GetPort());
		return -65535;
	}
	return port;
}

int RTSPServer::GetPort()
{
	return gst_rtsp_server_get_bound_port(m_server);
}

void RTSPServer::SessionsSetMax(guint max)
{
	LogDebug("RTSPServer::SessionsSetMax(%u)", max);

	GstRTSPSessionPool *Pool = gst_rtsp_server_get_session_pool(m_server);
	gst_rtsp_session_pool_set_max_sessions(Pool, max);
	g_object_unref(Pool);
}

guint RTSPServer::SessionsGetMax()
{
	LogDebug("RTSPServer::SessionsGetMax()");

	guint max = 0;
	GstRTSPSessionPool *Pool = gst_rtsp_server_get_session_pool(m_server);
	max = gst_rtsp_session_pool_get_max_sessions(Pool);
	g_object_unref(Pool);
	return max;
}

guint RTSPServer::SessionsCount()
{
	LogDebug("RTSPServer::SessionsCount()");

	guint current = 0;
	GstRTSPSessionPool *Pool = gst_rtsp_server_get_session_pool(m_server);
	current = gst_rtsp_session_pool_get_n_sessions(Pool);
	g_object_unref(Pool);
	return current;
}

void RTSPServer::BacklogSet(guint max)
{
	LogDebug("RTSPServer::BacklogSet(%u)", max);
	gst_rtsp_server_set_backlog(m_server, max);
}

guint RTSPServer::BacklogGet()
{
	int value = gst_rtsp_server_get_backlog(m_server);
	LogDebug("RTSPServer::BacklogGet(%u)", value);
	return value;
}

bool RTSPServer::ConfigLoad(Json::Value &json)
{
	LogDebug("RTSPServer::ConfigLoad");

	if (json.isMember("backlog") && json["backlog"].isNumeric())
		BacklogSet(json["backlog"].asInt());

	if (json.isMember("maxsessions") && json["maxsessions"].isNumeric())
		SessionsSetMax(json["maxsessions"].asInt());

	if (json.isMember("port") && json["port"].isNumeric())
	{
		if (SetPort(json["massessison"].asInt()) < 0)
		{
			LogError("RTSPServer::ConfigLoad - Failed to set port");
		}
	}

	return true;
}

bool RTSPServer::ConfigSave(Json::Value &json)
{
	LogDebug("RTSPServer::ConfigSave");

	json["backlog"] = BacklogGet();
	json["maxsessions"] = SessionsGetMax();
	json["port"] = GetPort();

	return true;
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

