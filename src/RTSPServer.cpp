
#include <Camera.h>

static GstElement *(*default_pipeline) (GstRTSPMediaFactory *factory, const GstRTSPUrl *url);

static GstElement *transcode_pipeline(GstRTSPMediaFactory *factory, const GstRTSPUrl *url)
{
	LogDebug("transcode_pipeline %s %s", url->abspath, url->query);
	if (factory == NULL || url == NULL || url->query == NULL)
		return default_pipeline(factory, url);

	std::string path = url->abspath;
	path = String::ToLower(url->abspath);
	std::vector<std::string> vec;
	if (String::Split(&path, "/", &vec) == false)
		return default_pipeline(factory, url);
	if (vec.size() != 4)
		return default_pipeline(factory, url);
	if (vec[1] != "video")	
		return default_pipeline(factory, url);
	if (vec[3] != "transcode")
		return default_pipeline(factory, url);

	int vinput = atoi(vec[2].c_str());
	LogDebug("TransCode: %d", vinput);

	std::string query = url->query;
	std::map<std::string, std::string> qmap;
	if (String::Split(&query, "&", "=", &qmap) == false)
	{
		LogError("Invalid Paramaters");
		return NULL;
	}

	std::string codec;
	std::string codec_mode;
	int width = 0;
	int height = 0;
	int fps = 0;
	int quality = 0;
	int gop = 30;
	int bitrate = 512;
	
	if (qmap.find("codec") != qmap.end())
		codec = qmap["codec"];

	if (qmap.find("codec_mode") != qmap.end())
		codec_mode = qmap["codec_mode"];

	
	if (qmap.find("width") != qmap.end() && qmap.find("height") != qmap.end())
	{
		width = atoi(qmap["width"].c_str());
		height = atoi(qmap["height"].c_str());
	}
	
	if (qmap.find("fps") != qmap.end())
		fps = atoi(qmap["fps"].c_str());
		
	if (qmap.find("quality") != qmap.end())
		quality = atoi(qmap["quality"].c_str());
		
	if (qmap.find("gop") != qmap.end())
		gop = atoi(qmap["gop"].c_str());

	if (qmap.find("bitrate") != qmap.end())
		bitrate = atoi(qmap["bitrate"].c_str());

	LogDebug("TransCode Codec: %s Width: %d Height: %d FPS: %d Quality: %d GOP: %d BitRate: %d", codec.c_str(), width, height, fps, quality, gop, bitrate);

	std::stringstream pipe;
	
	if (codec == "mjpeg")
	{
		pipe << "(";
		pipe << " internalsrc streamname=video" << vinput << "raw";
		//pipe << " ! decodebin";
		if (width != 0 && height != 0)
			pipe << " ! videoscale ! video/x-raw, width=" << width << ", height=" << height;
		if (fps > 0)
			pipe << " ! videorate ! video/x-raw, framerate=" << fps << "/1";
		if (fps < 0)
			pipe << " ! videorate ! video/x-raw, framerate=1/" << abs(fps);
		pipe << " ! videoconvert";
		if (quality > 0 && quality < 100)
			pipe << " ! jpegenc quality=" << quality;
		else
			pipe << " ! jpegenc";
		pipe << " ! rtpjpegpay name=pay0 pt=96";
		pipe << " )";
	}
	else if (codec == "h264")
	{
		pipe << "(";
		pipe << " internalsrc streamname=video" << vinput << "raw";
		//pipe << " ! decodebin";
		if (width != 0 && height != 0)
			pipe << " ! videoscale ! video/x-raw, width=" << width << ", height=" << height;
		if (fps > 0)
			pipe << " ! videorate ! video/x-raw, framerate=" << fps << "/1";
		if (fps < 0)
			pipe << " ! videorate ! video/x-raw, framerate=1/" << abs(fps);
		pipe << " ! videoconvert";
		if (quality > 0 && quality < 100)
		{
			pipe << " ! x264enc key-int-max=" << gop << " pass=5 quantizer=" << quality / 2;
		}
		else
		{
			if (codec_mode == "cbr")
			{
				pipe << " ! x264enc key-int-max=" << gop << " pass=0 bitrate=" << bitrate;			
			}
			else if (codec_mode == "vbr")
			{
				pipe << " ! x264enc key-int-max=" << gop << " pass=17 bitrate=" << bitrate;			
			}
			else
			{
				pipe << " ! x264enc key-int-max=" << gop << " bitrate=" << bitrate;
			}
		}
		pipe << " ! h264parse ! rtph264pay name=pay0 pt=96";
		pipe << " )";
	}
	else
	{
		LogError("transcode unknown codec %s", codec.c_str());
	}

	LogDebug("Using Pipeline: %s", pipe.str().c_str());

	GError *error = NULL;
	GstElement *element = gst_parse_launch (pipe.str().c_str(), &error);
	if (error != NULL)
	{
		LogError("transcode_pipeline: Parse Error: %s", error->message);
		g_error_free(error);
	}
	if (element == NULL)
	{
		LogError("transcode_pipeline failed to make pipeline from %s", pipe.str().c_str());
	}

	LogDebug("Made pipelin");
	return element;
}

RTSPServer::RTSPServer()
{
	LogDebug("RTSPServer::RTSPServer");
	m_loop = NULL;
	m_server = NULL;
	m_auth = NULL;
	m_port = 8554;
	Thread::Start();
	m_startbar.Wait();
	
	//FIXME: This is ugly. Need a new glib subclass
	GstRTSPMediaFactory *factory = gst_rtsp_media_factory_new ();
	default_pipeline = GST_RTSP_MEDIA_FACTORY_GET_CLASS(factory)->create_element;
	GST_RTSP_MEDIA_FACTORY_GET_CLASS(factory)->create_element = transcode_pipeline;
	g_object_unref(factory);
}

RTSPServer::~RTSPServer()
{
	LogDebug("RTSPServer::~RTSPServer");
	while(!g_main_loop_is_running(m_loop)) { } //Glib Sucks!
	g_main_loop_quit(m_loop);
	Thread::Stop();
}

void RTSPServer::VideoPipelineAdd(const std::string url, const std::string pipeline)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("RTSPServer::VideoPipelineAdd(%s, %s)", url.c_str(), pipeline.c_str());

	std::map<std::string, std::string>::iterator it = m_videopipelines.find(url);
	if (it != m_videopipelines.end())
	{
		LogCritical("RTSPServer::VideoPipelineAdd - url '%s' already exists", url.c_str());
		abort();
	}

	GstRTSPMountPoints *mounts = gst_rtsp_server_get_mount_points (m_server);
	
	GstRTSPMediaFactory *factory = gst_rtsp_media_factory_new ();
	gst_rtsp_media_factory_add_role (factory, "RTSP",
		GST_RTSP_PERM_MEDIA_FACTORY_ACCESS, G_TYPE_BOOLEAN, TRUE,
		GST_RTSP_PERM_MEDIA_FACTORY_CONSTRUCT, G_TYPE_BOOLEAN, TRUE, NULL);

	gst_rtsp_media_factory_set_launch (factory, pipeline.c_str());
	gst_rtsp_media_factory_set_shared (factory, TRUE);
	gst_rtsp_mount_points_add_factory (mounts, url.c_str(), factory);

	GstRTSPMediaFactory *transcode = gst_rtsp_media_factory_new ();
	gst_rtsp_media_factory_add_role (transcode, "RTSP",
		GST_RTSP_PERM_MEDIA_FACTORY_ACCESS, G_TYPE_BOOLEAN, TRUE,
		GST_RTSP_PERM_MEDIA_FACTORY_CONSTRUCT, G_TYPE_BOOLEAN, TRUE, NULL);

	std::string transurl = url + "/transcode";
	gst_rtsp_media_factory_set_launch (transcode, "xyz");
	gst_rtsp_media_factory_set_shared (transcode, TRUE);
	gst_rtsp_mount_points_add_factory (mounts, transurl.c_str(), transcode);


	g_object_unref(mounts);

	m_videopipelines[url] = pipeline;
}

void RTSPServer::VideoPipelineRemove(const std::string url)
{
	std::string transurl = url + "/transcode";
	ScopedLock lock = ScopedLock(&m_mutex);
	LogDebug("RTSPServer::VideoPipelineRemove(%s)", url.c_str());

	std::map<std::string, std::string>::iterator it = m_videopipelines.find(url);
	if (it == m_videopipelines.end())
	{
		LogCritical("RTSPServer::VideoPipelineRemove - url '%s' does not exist", url.c_str());
		abort();
	}

	GstRTSPMountPoints *mounts = gst_rtsp_server_get_mount_points (m_server);
	gst_rtsp_mount_points_remove_factory(mounts, url.c_str());
	gst_rtsp_mount_points_remove_factory(mounts, transurl.c_str());
	g_object_unref (mounts);
	m_videopipelines.erase(it);	
}

static GstRTSPFilterResult KickAllFilter (GstRTSPServer *server, GstRTSPClient *client, gpointer user_data)
{
	return GST_RTSP_FILTER_REMOVE;
}

void RTSPServer::KickAll()
{
	TimerAbort abrt = TimerAbort(120);
	CameraTimers->Add(&abrt); //This hangs sometime because of a bug in gst-rtsp-server

	ScopedLock lock = ScopedLock(&m_mutex);
	LogInfo("RTSPServer::KickAll all RTSP Clients");
	GList *lst = gst_rtsp_server_client_filter(m_server, KickAllFilter, NULL);
	g_list_free(lst);
	LogInfo("RTSPServer::KickAll Finished");

	CameraTimers->Remove(&abrt);
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

	if (port < 1024 && Caps::HasCap(CAP_NET_BIND_SERVICE) <= 0 && getuid() != 0)
	{
		LogInfo("RTSPServer::SetPort Don't have CAP_NET_BIND_SERVICE not trying to switch to port '%d'", port);
		return -EPERM;
	}

	//Take a copy of our url + pipelines
	std::map<std::string, std::string> videopipelines = m_videopipelines;
	m_videopipelines.clear();
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
	std::map<std::string, std::string>::iterator it = videopipelines.begin();
	while(it != videopipelines.end())
	{
		VideoPipelineAdd(it->first, it->second);
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

static gboolean Authenticate(GstRTSPAuth *auth, GstRTSPContext *ctx)
{
	LogInfo("RTSPServer::Authenticate");
	gchar *info;
	if (gst_rtsp_message_get_header (ctx->request, GST_RTSP_HDR_AUTHORIZATION, &info, 0) < 0)
	{
		LogDebug("RTSPServer::Authenticate - No Header");
		return TRUE;
	}

	if (g_ascii_strncasecmp (info, "basic ", 6) == 0) {
		gsize len = 0;
		g_base64_decode_inplace(&info[6], &len);
		info[len + 6] = 0;

		std::string str = &info[6];
		std::string Username = "";
		std::string Password = "";
		if (String::SplitOne(&str, &Username, &Password, ":") == false)
		{
			LogWarning("RTSPServer::Authenticate - Failed to parse username / password");
			return FALSE;
		}
		if (User::Auth(Username, Password) == false)
		{
			LogWarning("RTSPServer::Authenticate - Failed to auth");
			return FALSE;
		}

/*
		if (Group::IsUserInGroup("RTSP", Username) == false && 
			Group::IsUserInGroup("admin", Username) == false)
		{
			LogWarning("RTSPServer::Authenticate - Failed to auth user '%s' is not in group RTSP,admin", Username.c_str());
			return false;
		}
*/
		LogInfo("RTSPServer::Authenticate - Success");
		
		GstRTSPToken *token = gst_rtsp_token_new(GST_RTSP_TOKEN_MEDIA_FACTORY_ROLE, G_TYPE_STRING, "RTSP", NULL);
		ctx->token = token;
		
		return TRUE;
	} else if(g_ascii_strncasecmp (info, "digest ", 7) == 0)
	{
		LogWarning("RTSPServer::Authenticate - Unknown Auth Mode 'digest'");
		return FALSE;
	}
	else
	{
		LogWarning("RTSPServer::Authenticate - Unknown Auth Mode '%s'", info);
		return FALSE;
	}
	

	return FALSE;
}

void RTSPServer::Run()
{
	RTSPServerCleanup PoolCleaner;

	m_loop = g_main_loop_new (NULL, FALSE);
	m_server = gst_rtsp_server_new ();
	m_auth = gst_rtsp_auth_new ();
	gst_rtsp_server_set_auth (m_server, m_auth);
	
	if (GST_IS_RTSP_AUTH(m_auth) == false)
		abort();
	GST_RTSP_AUTH_GET_CLASS(m_auth)->authenticate = Authenticate;


	if (m_port != 0)
	{
		std::stringstream ss;
		ss << m_port;
		gst_rtsp_server_set_service(m_server, ss.str().c_str());
	}

	guint id = gst_rtsp_server_attach (m_server, g_main_loop_get_context(m_loop));
	if (id == 0)
	{
		LogError("RTSPServer::Run - Server failed to attach");
		LogError("RTSPServer::Run - RTSP Is NOT running");
		m_startbar.WakeUp();
		g_main_loop_run(m_loop);
	}
	else
	{
		PoolCleaner.Start(m_server);

		m_startbar.WakeUp();

		int cport = gst_rtsp_server_get_bound_port(m_server);
		LogInfo("RTSPServer::Run - Running On Port %d", cport);

		g_main_loop_run(m_loop);
		LogInfo("RTSPServer::Run - exiting Loop");

		//Same as missing api gst_rtsp_server_detach
		if (g_source_remove(id) == false)
		{
			LogError("RTSPServer::Run - Failed to remove g_source");
			abort();
		}

		PoolCleaner.Stop();
	}

	g_object_unref(m_server);
	g_object_unref (m_auth);
	g_main_loop_unref(m_loop);
	m_server = NULL;
	m_auth = NULL;	
	m_loop = NULL;
}

