
#include <Camera.h>

PipelineBasic::PipelineBasic(const std::string str)
{
	if (pthread_mutex_init(&lock, NULL) != 0)
		abort();
	m_running = false;
	m_pipelinestr = str;
	m_restartdelay.tv_sec = 1;
	m_restartdelay.tv_usec = 0;
}

PipelineBasic::~PipelineBasic()
{
	if (pthread_mutex_destroy(&lock))
		abort();
}

void PipelineBasic::Start()
{
	m_running = true;
	if(pthread_create(&m_thread, NULL, &Run, this))
		abort();
}

void PipelineBasic::Stop()
{
	void *retval = NULL;
	m_running = false;
	if (pthread_join(m_thread, &retval))
		abort();
}

void *PipelineBasic::Run(void *arg)
{
	PipelineBasic *self = (class PipelineBasic *) arg;
	GstElement *pipeline = NULL;

	while(self->m_running)
	{
		GError *error = NULL;
		pipeline = gst_parse_launch (self->m_pipelinestr.c_str(), &error);
		if (!pipeline) {
			LogError("Cannot Parse: %s", self->m_pipelinestr.c_str());
			LogError("Parse error: %s", error->message);
			LogCritical("Giving Up on pipeling: %s", self->m_pipelinestr.c_str());
			//Not going to work the next time either so just bail.
			return NULL;
		}

		if (GstUtil::SetState(pipeline, GST_STATE_PLAYING) == true)
		{
			while (GstUtil::WaitForEos(pipeline) == false && self->m_running == true) { }
		}
		else
		{
			LogError("Failed To Start Pipeline: %s", self->m_pipelinestr.c_str());
		}

		if (GstUtil::SetState(pipeline, GST_STATE_NULL) == false)
		{
			LogCritical("Failed to Set pipeline state to NULL");
			abort();
		}

		gst_object_unref(pipeline);
		pipeline = NULL;
		LogWarning("Pipeline Stopped: %s", self->m_pipelinestr.c_str());
		Time::Sleep(&self->m_restartdelay);
	}
	return NULL;
}


