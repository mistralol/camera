
#include <Camera.h>

WebStreamPipe::WebStreamPipe(const std::string strpipe) :
	PipelineBasic(strpipe),
	m_Current({0, 0}),
	m_TotalOverRun(0), m_TotalPushed(0),
	m_CurrentOverRun(0), m_CurrentPushed(0)
{

}

WebStreamPipe::~WebStreamPipe()
{

}

void WebStreamPipe::OnStart(GstElement *pipeline)
{
	LogDebug("WebStreamPipe::OnStart(%p)", pipeline);
	
	GstElement *queue = gst_bin_get_by_name((GstBin *) pipeline, "queue");
	if (queue)
	{
		g_signal_connect(queue, "underrun", G_CALLBACK(OnQueueUnderRun), this);
		g_signal_connect(queue, "running", G_CALLBACK(OnQueueRunning), this);
		g_signal_connect(queue, "overrun", G_CALLBACK(OnQueueOverRun), this);
		g_signal_connect(queue, "pushing", G_CALLBACK(OnQueuePushing), this);
		gst_object_unref(queue);
	}
	else
	{
		LogDebug("WebStreamPipe::OnStart - No Queue element");
	}
}

void WebStreamPipe::Update()
{
	if (m_Current.tv_sec == 0)
	{
		Time::MonoTonic(&m_Current);
	}
	else
	{
		struct timespec delay = {3, 0};
		struct timespec now;
		struct timespec then;
		Time::MonoTonic(&now);
		Time::Add(&m_Current, &delay, &then);
		if (Time::IsGreater(&now, &then))
		{
			LogDebug("WebStreamPipe::Update CurrentOverRun = %d CurrentPushed = %d", m_CurrentOverRun, m_CurrentPushed);
			if (m_CurrentOverRun > 0)
			{
				//Find drop ratio
				float f = (float) m_CurrentPushed / (float) m_CurrentOverRun;
				LogDebug("f = %f\n", f);
			
				ScaleBitRate(0.8f);
			}
			else
			{
				ScaleBitRate(1.01f);
			}
		
			m_CurrentOverRun = 0;
			m_CurrentPushed = 0;
			Time::MonoTonic(&m_Current);
		}
	}
}

void WebStreamPipe::ScaleBitRate(float s)
{
	GstElement *encoder = gst_bin_get_by_name((GstBin *) m_pipeline, "encoder");
	if (encoder)
	{
		guint bitrate = 0;
		g_object_get(G_OBJECT(encoder), "bitrate", &bitrate, NULL);
		float b = bitrate;
		b *= s;
		guint newbitrate = b;
		if (newbitrate < 1024)
		{
			LogDebug("Changing BitRate OLD: %u NEW: %u Scaled: %f", bitrate, newbitrate, s);
			g_object_set(G_OBJECT(encoder), "bitrate", newbitrate, NULL);
			gst_object_unref(encoder);
		}
	}
	else
	{
		LogDebug("Cant find encoder");
	}
}

void WebStreamPipe::OnQueueUnderRun(GstElement *, gpointer userData)
{
	//LogDebug("WebStreamPipe::OnQueueUnderRun");
}

void WebStreamPipe::OnQueueRunning(GstElement *, gpointer userData)
{
	//LogDebug("WebStreamPipe::OnQueueRunning");
}

void WebStreamPipe::OnQueueOverRun(GstElement *, gpointer userData)
{
	WebStreamPipe *self = (WebStreamPipe *) userData;
	//LogDebug("WebStreamPipe::OnQueueOverRun");
	self->m_CurrentOverRun++;
	self->Update();
}

void WebStreamPipe::OnQueuePushing(GstElement *, gpointer userData)
{
	WebStreamPipe *self = (WebStreamPipe *) userData;
	//LogDebug("WebStreamPipe::OnQueuePushing");
	self->m_CurrentPushed++;
	self->Update();
}



