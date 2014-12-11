
//static only helper functions for common gstreamer functions
class GstUtil
{
	public:
		static bool WaitForEos(GstElement *pipeline);
		static bool WaitForEos(GstElement *pipeline, GstBus *bus);
		static bool SetState(GstElement *pipeline, GstState state);
};

