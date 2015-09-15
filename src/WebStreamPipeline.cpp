
#include <Camera.h>

WebStreamPipeline::WebStreamPipeline(WebStreamOptions *options) :
	m_fd(-1), m_strpipe(""), m_finished(false)
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

	Thread::Start();
	return port;
}

void WebStreamPipeline::Stop()
{
	Thread::Stop();
}

bool WebStreamPipeline::HasFinished()
{
	return m_finished;
}

void WebStreamPipeline::Run()
{
	
	//Use select to wait for a connection
	
	//Accept connection
	
	//Start gstreamer pipeline
	
	//Clear up
	m_finished = true;
}



