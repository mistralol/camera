
#include <Camera.h>

RTSPServer::RTSPServer()
{
	LogDebug("RTSPServer::RTSPServer");
	m_loop = NULL;
	m_server = NULL;
	m_port = 8554;
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
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("RTSPServer::PipelineAdd(%s, %s)", url.c_str(), pipeline.c_str());

	std::map<std::string, std::string>::iterator it = m_urls.find(url);
	if (it != m_urls.end())
	{
		LogCritical("RTSPServer::PipelineAdd - url '%s' already exists", url.c_str());
		abort();
	}

	GstRTSPMountPoints *mounts = gst_rtsp_server_get_mount_points (m_server);
	GstRTSPMediaFactory *factory = gst_rtsp_media_factory_new ();

	gst_rtsp_media_factory_set_launch (factory, pipeline.c_str());
	gst_rtsp_media_factory_set_shared (factory, TRUE);
	gst_rtsp_mount_points_add_factory (mounts, url.c_str(), factory);

	g_object_unref(mounts);

	m_urls[url] = pipeline;
}

void RTSPServer::PipelineRemove(const std::string url)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("RTSPServer::PipelineRemove(%s)", url.c_str());

	std::map<std::string, std::string>::iterator it = m_urls.find(url);
	if (it == m_urls.end())
	{
		LogCritical("RTSPServer::PipelineRemove - url '%s' does not exist", url.c_str());
		abort();
	}

	GstRTSPMountPoints *mounts = gst_rtsp_server_get_mount_points (m_server);
	gst_rtsp_mount_points_remove_factory(mounts, url.c_str());
	g_object_unref (mounts);
	m_urls.erase(it);	
}

int RTSPServer::SetPort(int port)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("RTSPServer::SetPort(%d)", port);
	if (port <= 0 || port > 65535)
	{
		LogError("RTSPServer::SetPort(%d) - Value out of range <= 0 || > 65535", port);
		return -EINVAL;
	}

	if (GetPort() == port)
	{
		LogInfo("RTSPServer::SetPort Port Already Set To %d - No work todo!", port);
		return port;
	}

	//Take a copy of our url + pipelines
	std::map<std::string, std::string> urls = m_urls;
	m_urls.clear();
	guint backlog = BacklogGet();

	//gst rtsp services do not permit us to change the when the service is running so we need to restart it
	//then load the pipelines back into the service

	//Stop Services
	while(!g_main_loop_is_running(m_loop))
	{
		//This spin loop exists because the loop won't quit is quit is called before g_main_loop_run
		//That is because glib sucks ass
		//This is because the glib api sucks and does not give a way to protect against this sort of issue
	}

	g_main_loop_quit(m_loop);
	Thread::Stop();
	m_startbar.Reset();

	//Change the port
	m_port = port;

	//Start Services
	Thread::Start();
	m_startbar.Wait();

	//Put pipelines back
	std::map<std::string, std::string>::iterator it = urls.begin();
	while(it != urls.end())
	{
		PipelineAdd(it->first, it->second);
		it++;
	}

	BacklogSet(backlog);

	if (GetPort() == port)
	{
		LogInfo("RTSPServer::SetPort - Port changed to %d Success!", port);
		return port;
	}

	LogInfo("RTSPServer::SetPort - Failed with unknown error");
	return -1;
}

int RTSPServer::GetPort()
{
	ScopedLock lock = ScopedLock(&m_mutex);
	return gst_rtsp_server_get_bound_port(m_server);
}

void RTSPServer::SessionsSetMax(guint max)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("RTSPServer::SessionsSetMax(%u)", max);

	GstRTSPSessionPool *Pool = gst_rtsp_server_get_session_pool(m_server);
	gst_rtsp_session_pool_set_max_sessions(Pool, max);
	g_object_unref(Pool);
}

guint RTSPServer::SessionsGetMax()
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("RTSPServer::SessionsGetMax()");

	guint max = 0;
	GstRTSPSessionPool *Pool = gst_rtsp_server_get_session_pool(m_server);
	max = gst_rtsp_session_pool_get_max_sessions(Pool);
	g_object_unref(Pool);
	return max;
}

guint RTSPServer::SessionsCount()
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("RTSPServer::SessionsCount()");
	guint current = 0;
	GstRTSPSessionPool *Pool = gst_rtsp_server_get_session_pool(m_server);
	current = gst_rtsp_session_pool_get_n_sessions(Pool);
	g_object_unref(Pool);
	return current;
}

void RTSPServer::BacklogSet(guint max)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("RTSPServer::BacklogSet(%u)", max);
	gst_rtsp_server_set_backlog(m_server, max);
}

guint RTSPServer::BacklogGet()
{
	ScopedLock lock = ScopedLock(&m_mutex);
	int value = gst_rtsp_server_get_backlog(m_server);
	LogDebug("RTSPServer::BacklogGet(%u)", value);
	return value;
}

bool RTSPServer::ConfigLoad(Json::Value &json)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("RTSPServer::ConfigLoad");

	if (json.isMember("backlog") && json["backlog"].isNumeric())
		BacklogSet(json["backlog"].asInt());

	if (json.isMember("maxsessions") && json["maxsessions"].isNumeric())
		SessionsSetMax(json["maxsessions"].asInt());

	if (json.isMember("port") && json["port"].isNumeric())
	{
		if (SetPort(json["port"].asInt()) < 0)
		{
			LogError("RTSPServer::ConfigLoad - Failed to set port");
		}
	}

	return true;
}

bool RTSPServer::ConfigSave(Json::Value &json)
{
	ScopedLock lock = ScopedLock(&m_mutex);
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

	if (m_port != 0)
	{
		std::stringstream ss;
		ss << m_port;
		gst_rtsp_server_set_service(m_server, ss.str().c_str());
	}

	gst_rtsp_server_attach (m_server, NULL);
	PoolCleaner.Init(m_server);

	m_startbar.WakeUp();

	int cport = gst_rtsp_server_get_bound_port(m_server);
	LogInfo("RTSPServer::Run - Running On Port %d", cport);

	g_main_loop_run(m_loop);
	LogInfo("RTSPServer::Run - exiting Loop");

	g_object_unref(m_server);
	g_main_loop_unref(m_loop);
	m_server = NULL;
	m_loop = NULL;
}

