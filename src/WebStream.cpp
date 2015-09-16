
#include <Camera.h>

WebStream::WebStream()
{
	m_TmrCleanup = new TimerFunc(60, Cleanup, this);
	CameraTimers->Add(m_TmrCleanup);
}

WebStream::~WebStream()
{
	CameraTimers->Remove(m_TmrCleanup);
	delete m_TmrCleanup;
	
	//Don't need lock because we *should be the only owner*
	//Kill all pipelines
	for(std::list<WebStreamPipeline *>::iterator it = m_pipelines.begin(); it != m_pipelines.end(); it++)
	{
		WebStreamPipeline *pipe = *it;
		pipe->Stop();
		delete pipe;
	}
	m_pipelines.clear();
}

int WebStream::StartVideoInput(WebStreamOptions *options)
{
	LogDebug("WebStream::StartVideoInput()");
	WebStreamPipeline *pipe = new WebStreamPipeline(options);
	int port = pipe->Start();
	if (port < 0)
	{
		delete pipe;
		return port;
	}
	m_mutex.Lock();
	m_pipelines.push_back(pipe);
	m_mutex.Unlock();
	return port;
}

void WebStream::Cleanup(void *arg)
{
	WebStream *self = (WebStream *) arg;
	LogDebug("WebStream::Cleanup()");
	
restart:
	self->m_mutex.Lock();
	for(std::list<WebStreamPipeline *>::iterator it = self->m_pipelines.begin(); it != self->m_pipelines.end(); it++)
	{
		WebStreamPipeline *pipe = *it;
		if (pipe->HasFinished())
		{
			self->m_pipelines.erase(it);
			self->m_mutex.Unlock();
			pipe->Stop();
			LogDebug("WebStream::Cleanup - Removed %p", pipe);
			delete pipe;
			goto restart;
		}
	}
	self->m_mutex.Unlock();
	CameraTimers->Add(self->m_TmrCleanup);
}


