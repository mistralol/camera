
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
	struct timespec timeout = { m_options.timeout, 0};
	struct pollfd fds;
	
	fds.fd = m_fd;
	fds.events = POLLIN;

restart_poll:
	int ret = ppoll(&fds, 1, &timeout, NULL);
	if (ret < 0)
	{
		switch(errno)
		{
			case ETIMEDOUT:
				m_finished = true;
				return;
				break;
			case EINTR:
				goto restart_poll;
				return;
			default:
				LogError("WebStreamPipeline::Run() - Unhandler error waiting for connection '%s'", strerror(errno));
				m_finished = true;
				return;
				break;
		}
	}

	if ((fds.revents & POLLIN) == 0)
	{
		LogError("WebStreamPipeline::Run() - Poll returned but did not have POLLIN set");
		m_finished = true;
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
	
	if (m_options.type == "Unknown")
	{
		LogError("WebStreamPipeline::Run() - Unknown format requested");
		goto cleanup;
	}
	else if (m_options.type == "MP4")
	{
		ss << "internalsrc streamname=video" << m_options.vinput;
		ss << " ! h264parse";
		ss << " ! qtmux streamable=true fragment-duration=250";
		ss << " ! fdsink fd=" << fd;
	}
	else if (m_options.type == "MKV")
	{
		ss << "internalsrc streamname=video" << m_options.vinput;
		ss << " ! h264parse";
		ss << " ! matroskamux streamable=true";
		ss << " ! fdsink fd=" << fd;
	}
	else if (m_options.type == "MKV_TRANS")
	{
		ss << "internalsrc streamname=video" << m_options.vinput;
		ss << " ! h264parse ! decodebin ";
		ss << " ! videorate ! video/x-raw, framerate=15/1 ";
		ss << " ! queue name=queue leaky=1 max-size-buffers=5";
		ss << " ! x264enc name=encoder tune=4 key-int-max=1 bitrate=1024";
		ss << " ! matroskamux streamable=true";
		ss << " ! fdsink fd=" << fd;
	}
	else if (m_options.type == "WEBM")
	{
		ss << "internalsrc streamname=video" << m_options.vinput;
		ss << " ! webmmux streamable=true";
		ss << " ! fdsink fd=" << fd;
	}
	else if (m_options.type == "MJPEG")
	{
		ss << "internalsrc streamname=video" << m_options.vinput;
		ss << " ! multipartmux boundary=boundary";
		ss << " ! fdsink fd=" << fd;
	}
	else
	{
		LogCritical("WebStreamPipeline::Run() - Bad value or unsuported type %s", m_options.type.c_str());
		goto cleanup;
	}
/*
		case MJPEG_TRANS:
			ss << "internalsrc streamname=video" << m_options.vinput;
			ss << " ! decodebin";
			if (m_options.mjpeg_fps > 0 && m_options.mjpeg_fps < 30)
				ss << " ! videorate ! video/x-raw, framerate=" << m_options.mjpeg_fps << "/1";
			//< 0 means fractions eg 1/abs(fps)
			if (m_options.mjpeg_fps < 0)
				ss << " ! videorate ! video/x-raw, framerate=1/" << m_options.mjpeg_fps * -1;
			if (m_options.mjpeg_quality > 0)
			{
				if (m_options.mjpeg_quality > 100)
					m_options.mjpeg_quality = 100;
				ss << " ! jpegenc quality=" << m_options.mjpeg_quality;
			}
			else
			{
				ss << " ! jpegenc";
			}
			ss << " ! multipartmux boundary=boundary";
			ss << " ! fdsink fd=" << fd;
			break;
*/


	//Start gstreamer pipeline
	do {
		LogDebug("Starting Pipeline '%s'", ss.str().c_str());
		std::unique_ptr<WebStreamPipe> Pipeline(new WebStreamPipe(ss.str()));
		Pipeline->SetName("WebStream");
		Pipeline->SetRestart(false);
		Pipeline->Start();
		Pipeline->WaitForExit();		
	} while(0);
	
	//Clear up
cleanup:
	if (close(fd) < 0)
		close(fd);
	m_finished = true;
}



