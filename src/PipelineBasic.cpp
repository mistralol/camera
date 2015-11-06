
#include <Camera.h>

PipelineBasic::PipelineBasic(const std::string str)
{
	m_running = false;
	m_pipelinestr = str;
	m_restart = true;
	m_restartdelay.tv_sec = 1;
	m_restartdelay.tv_nsec = 0;
	m_exited = false;
	m_name = "";
}

PipelineBasic::~PipelineBasic()
{
	if (m_running == true)
		abort(); //Somebody forgot to call stop
}

void PipelineBasic::SetName(const std::string name)
{
	m_name = name;
}

void PipelineBasic::SetRestart(bool value)
{
	m_restart = value;
}

void PipelineBasic::SetRestartDelay(const struct timespec *ts)
{
	m_restartdelay = *ts;
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
	ScopedLock lock(&m_mutex);
	if (m_pipeline)
	{
		//Kick Pipeline
		SetState(m_pipeline, GST_STATE_PAUSED);
	}
	m_running = false;
	m_mutex.WakeUpAll();
	lock.Unlock();

	if (pthread_join(m_thread, &retval))
		abort();
}

void PipelineBasic::WaitForExit()
{
	ScopedLock lock(&m_mutex);
	while(m_exited == false)
	{
		m_mutex.Wait();
	}
}

void PipelineBasic::OnParseError(const gchar *msg)
{
	LogError("Pipeline '%s' Cannot Parse: %s", m_name.c_str(), m_pipelinestr.c_str());
	LogError("Pipeline '%s' Parse error: %s", m_name.c_str(), msg);
}

void PipelineBasic::OnStart(GstElement *pipeline)
{
	LogDebug("Pipeline '%s' Started", m_name.c_str());
}

void PipelineBasic::OnStartFailure(GstElement *pipeline)
{
	LogError("Pipeline '%s' Failed To Start", m_name.c_str());
}

void PipelineBasic::OnStop(GstElement *pipeline)
{
	LogDebug("Pipeline '%s' Stopped", m_name.c_str());
}

void PipelineBasic::OnError(const gchar *msg, const gchar *details)
{
	LogError("Pipeline '%s', ERROR: %s Details: %s", m_name.c_str(), msg, details);
}

void PipelineBasic::OnExit()
{
	LogDebug("Pipeline '%s' Exited", m_name.c_str());
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
			self->OnParseError(error->message);

			//Not going to ever work bail so exit	
			ScopedLock lock(&self->m_mutex);
			self->m_exited = true;
			self->m_mutex.WakeUpAll();
			return NULL;
		}
		
		//Export pipeline
		self->m_mutex.Lock();
		self->m_pipeline = pipeline;
		self->m_mutex.Unlock();
		
		self->OnStart(pipeline);

		if (self->SetState(pipeline, GST_STATE_PLAYING) == true)
		{
			GstBus *bus = gst_element_get_bus(pipeline);
			while (self->WaitForEos(pipeline, bus) == false && self->m_running == true)
			{
				//Idle Loop
			}
			gst_object_unref(bus);
		}
		else
		{
			self->OnStartFailure(pipeline);
		}
		
		self->OnStop(pipeline);

		if (self->SetState(pipeline, GST_STATE_NULL) == false)
		{
			LogCritical("Pipeline '%s' Failed to Set pipeline state to NULL", self->m_name.c_str());
			abort();
		}

		//Un export pipeline
		self->m_mutex.Lock();
		self->m_pipeline = NULL;
		self->m_mutex.Unlock();
		
		gst_object_unref(pipeline);
		pipeline = NULL;

		//Take lock and see if we need to restart
		do {
			ScopedLock lock(&self->m_mutex);
			if (self->m_restart == false)
			{
				self->m_exited = true;
				self->m_mutex.WakeUpAll();
				return NULL;
			}

			if (self->m_running)
				self->m_mutex.Wait(&self->m_restartdelay);
		} while(0);
	}
	
	do {
		ScopedLock lock(&self->m_mutex);
		self->m_exited = true;
		self->m_mutex.WakeUpAll();
	} while(0);

	return NULL;
}

bool PipelineBasic::SetState(GstElement *pipeline, GstState state)
{
	GstStateChangeReturn ret = gst_element_set_state (pipeline, state);
	if (ret == GST_STATE_CHANGE_SUCCESS)
		return true;
	if (ret == GST_STATE_CHANGE_FAILURE)
		return false;
	if (ret == GST_STATE_CHANGE_NO_PREROLL)
	{
		LogCritical("Pipeline '%s' GstUtil::SetState Unhandled Error Code GST_STATE_CHANGE_NO_PREROLL", m_name.c_str());
		abort(); //Not Yet Handled
	}

	if (ret == GST_STATE_CHANGE_ASYNC)
	{
		GstState curstate;
		GstClockTime timeout = GST_SECOND * 15;
		int retries = 1;

		for(int i =0;i<retries;i++)
		{
			ret = gst_element_get_state(pipeline, &curstate, NULL, timeout);
			if (ret == GST_STATE_CHANGE_SUCCESS)
				return true;
			if (ret == GST_STATE_CHANGE_FAILURE)
				return false;
			LogWarning("Pipeline '%s' gst_element_get_state timeout: %s", m_name.c_str(), gst_element_state_change_return_get_name(ret));
		}

		LogError("Pipeline '%s' GstUtil::SetState Lockup detected", m_name.c_str());
		return false;
	}
	else
	{
		LogCritical("Pipeline '%s' GstUtil::SetState Invalid Return from gst_element_set_state", m_name.c_str());
		abort(); //Accoridng to gstreamer documentation this is not reachable
	}
}

bool PipelineBasic::WaitForEos(GstElement *pipeline, GstBus *bus)
{
	GstClockTime timeout = 1000000000; //1 Seconds
	GstMessage *msg = gst_bus_timed_pop (bus, timeout); //We have to poll because this gstreamer function sometimes never returns
	if (msg == NULL)
	{
		GstState state;
		if (gst_element_get_state(pipeline, &state, NULL, GST_CLOCK_TIME_NONE) != GST_STATE_CHANGE_SUCCESS)
			return true;
		if (state != GST_STATE_PLAYING)
		{
			return true;
		}
		return false;
	}

	switch (GST_MESSAGE_TYPE (msg)) {
		case GST_MESSAGE_EOS:
			return true;
			break;
		case GST_MESSAGE_ERROR:
		{
			GError *err = NULL; /* error to show to users                 */
			gchar *dbg = NULL;  /* additional debug string for developers */
			gst_message_parse_error (msg, &err, &dbg);
			if (err && dbg) {
				OnError(err->message, dbg);
			}
			g_error_free (err);
			g_free(dbg);
			break;
		}
		//Suppress Noise
		case GST_MESSAGE_STREAM_STATUS:
		case GST_MESSAGE_ASYNC_START:
		case GST_MESSAGE_ASYNC_DONE:
		case GST_MESSAGE_STATE_CHANGED:
		case GST_MESSAGE_NEW_CLOCK:
			LogDebug("Pipeline '%s' UnHandled message of type %s", m_name.c_str(), GST_MESSAGE_TYPE_NAME (msg));
			break;
		default:
			LogDebug("Pipeline '%s' Unexpected message of type %s", m_name.c_str(), GST_MESSAGE_TYPE_NAME (msg));
			break;
	}
	gst_message_unref (msg);
	return false;
}


