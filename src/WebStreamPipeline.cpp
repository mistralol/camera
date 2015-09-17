
#include <Camera.h>

WebStreamPipeline::WebStreamPipeline(WebStreamOptions *options) :
	m_fd(-1), m_finished(false)
{
	m_options = *options;
}

WebStreamPipeline::~WebStreamPipeline()
{
	if (m_fd >= 0)
	{
		if (close(m_fd) < 0)
			abort();
		m_fd = -1;
	}
}

int WebStreamPipeline::Start()
{
	int port = -1;
	
	//Bind To Port
	m_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_fd < 0)
	{
		LogError("WebStreamPipeline::Start() - Cannot create socket '%s'", strerror(errno));
		return -errno;
	}
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	bool bound = false;
	
	for(int i =0;i<50;i++)
	{
		port = (rand() % 60000) + 1024;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = INADDR_ANY;
		if (m_options.localonly)
			addr.sin_addr.s_addr = inet_addr("127.0.0.1");

		int ret = bind(m_fd, (struct sockaddr *) &addr, sizeof(addr));
		if (ret < 0)
		{
			LogWarning("WebStreamPipeline::Start() - Failed to bind to port '%d' error '%s' on attempt number '%d'",
					port, strerror(errno), i);
			continue;
		}
		bound = true;
		LogDebug("WebStreamPipeline::Start() - Bound to port '%d'", port);
		break;
	}
	
	if (bound == false)
	{
		if (close(m_fd) < 0)
			abort();
		m_fd = -1;
		return -EADDRINUSE;
	}
	
	int ret = listen(m_fd, 1);
	if (ret < 0)
	{
		LogError("WebStreamPipeline::Start - Unable to listen on port '%d' error '%s'", port, strerror(errno));
		if (close(m_fd) < 0)
			abort();
		m_fd = -1;
		return -1;
	}

	m_running = true;
	m_finished = false;
	Thread::Start();
	return port;
}

void WebStreamPipeline::Stop()
{
	m_running = false;
	Thread::Stop();
}

bool WebStreamPipeline::HasFinished()
{
	return m_finished;
}

void WebStreamPipeline::Run()
{
	struct timeval timeout = { m_options.timeout, 0};
	fd_set rset;
	FD_ZERO(&rset);
	FD_SET(m_fd, &rset);
	
restart_accept:
	int ret = select(m_fd + 1, &rset, NULL, NULL, &timeout);
	if (ret < 0)
	{
		switch(errno)
		{
			case ETIMEDOUT:
				m_finished = true;
				return;
				break;
			case EINTR:
				goto restart_accept;
				return;
			default:
				LogError("WebStreamPipeline::Run() - Unhandler error waiting for connection '%s'", strerror(errno));
				m_finished = true;
				return;
				break;
		}
	}
	
	if (ret == 0)
	{
		LogWarning("WebStreamPipeline::Run() - Timeout while waiting for connection");
		m_finished = false;
		return;
	}

	//Accept connection	
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	int fd = accept4(m_fd, (struct sockaddr *) &addr, &len, SOCK_NONBLOCK | SOCK_CLOEXEC);
	if (fd < 0)
	{
		LogError("WebStreamPipeline::Run() - Unhandled error accepting connection '%s'", strerror(errno));
		m_finished = true;
		return;
	}
	LogInfo("WebStreamPipeline::Run() - Accepted Connection from '%s:%d'", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

	//We no longer need to listen
	if (close(m_fd) < 0)
		abort();
	m_fd = -1;

	//Make pipeline
	std::stringstream ss;
	
	switch(m_options.type)
	{
		case Unknown:
			LogError("WebStreamPipeline::Run() - Unknown format requested");
			goto cleanup;
			break;
		case MP4:
			ss << "internalsrc streamname=video" << m_options.vinput << " ! ";
			ss << "h264parse !";
			ss << "qtmux streamable=true fragment-duration=250 ! ";
			ss << "fdsink fd=" << fd;
			break;
		case FLV:
			ss << "internalsrc streamname=video" << m_options.vinput << " ! ";
			ss << "flvmux streamable=true ! ";
			ss << "fdsink fd=" << fd;
			break;
		default:
			LogCritical("WebStreamPipeline::Run() - Bad value or unsuported type");
			abort();
			break;
	}

	//Start gstreamer pipeline
	do {
		std::string pipe = ss.str();
		GstElement *pipeline = NULL;
		GError *error = NULL;
	
		pipeline = gst_parse_launch (pipe.c_str(), &error);
		if (!pipeline) {
			LogError("Cannot Parse: %s", pipe.c_str());
			LogError("Parse error: %s", error->message);
			goto cleanup;
		}
		
		//Enable for debugging
		//g_signal_connect(pipeline, "deep-notify", G_CALLBACK (gst_object_default_deep_notify), NULL);

		LogInfo("Starting Pipeline: '%s'", pipe.c_str());	
		if (GstUtil::SetState(pipeline, GST_STATE_PLAYING) == true)
		{
			while (GstUtil::WaitForEos(pipeline) == false && m_running == true) { }
		}
		else
		{
			LogError("Failed To Start Pipeline: %s", pipe.c_str());
		}

		if (GstUtil::SetState(pipeline, GST_STATE_NULL) == false)
		{
			LogCritical("WebStreamPipeline::Run() - Failed to Set pipeline state to NULL");
			abort();
		}
	
		LogInfo("Stopping Pipeline: '%s'", pipe.c_str());
		gst_object_unref(pipeline);
		pipeline = NULL;
	} while(0);
	
	//Clear up
cleanup:
	if (close(fd) < 0)
		close(fd);
	m_finished = true;
}



