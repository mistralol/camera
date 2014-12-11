
#include <Camera.h>

bool GstUtil::WaitForEos(GstElement *pipeline)
{
	GstBus *bus = gst_element_get_bus (pipeline);
	bool ret = WaitForEos(pipeline, bus);
	gst_object_unref (bus);
	return ret;
}

//Return true is pipeline is not in a running state
bool GstUtil::WaitForEos(GstElement *pipeline, GstBus *bus)
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
			if (err) {
				LogError("ERROR: %s\n", err->message);
				g_error_free (err);
			}
			if (dbg) {
				LogError("[Debug details: %s]\n", dbg);
				g_free(dbg);
			}

			break;
		}
		//Suppress Noise
		case GST_MESSAGE_STREAM_STATUS:
		case GST_MESSAGE_ASYNC_START:
		case GST_MESSAGE_ASYNC_DONE:
		case GST_MESSAGE_STATE_CHANGED:
		case GST_MESSAGE_NEW_CLOCK:
			break;
		default:
			LogDebug("Unexpected message of type %s\n", GST_MESSAGE_TYPE_NAME (msg));
			break;
	}
	gst_message_unref (msg);
	return false;
}

bool GstUtil::SetState(GstElement *pipeline, GstState state)
{
	GstStateChangeReturn ret = gst_element_set_state (pipeline, state);
	if (ret == GST_STATE_CHANGE_SUCCESS)
		return true;
	if (ret == GST_STATE_CHANGE_FAILURE)
		return false;
	if (ret == GST_STATE_CHANGE_NO_PREROLL)
	{
		LogCritical("GstUtil::SetState Unhandled Error Code GST_STATE_CHANGE_NO_PREROLL");
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
			LogWarning("gst_element_get_state timeout: %s", gst_element_state_change_return_get_name(ret));
		}

		LogError("GstUtil::SetState Lockup detected");
		return false;
	}
	else
	{
		LogCritical("GstUtil::SetState Invalid Return from gst_element_set_state");
		abort(); //Accoridng to gstreamer documentation this is not reachable
	}
}

