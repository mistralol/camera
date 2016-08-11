
class WebStreamPipe : public PipelineBasic
{
	public:
		WebStreamPipe(const std::string strpipe);
		~WebStreamPipe();

		virtual void OnStart(GstElement *pipeline);
		
		void Update();
		void ScaleBitRate(float s);
		
		static void OnQueueUnderRun(GstElement *, gpointer userData);
		static void OnQueueRunning(GstElement *, gpointer userData);
		static void OnQueueOverRun(GstElement *, gpointer userData);
		static void OnQueuePushing(GstElement *, gpointer userData);
		
	private:
		int m_TotalOverRun;
		int m_TotalPushed;
		
		struct timespec m_Current;
		int m_CurrentOverRun;
		int m_CurrentPushed;
		
};

