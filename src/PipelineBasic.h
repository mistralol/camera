

class PipelineBasic : public IPipeline
{
	public:
		PipelineBasic(const std::string strpipe);
		~PipelineBasic();

		void SetName(const std::string name);
		void SetRestart(bool value);
		void SetRestartDelay(const struct timespec *ts);

		void Start();
		void Stop();
		void WaitForExit();
		
		virtual void OnParseError(const gchar *msg);
		virtual void OnStart(GstElement *pipeline);
		virtual void OnStartFailure(GstElement *pipeline);
		
		virtual void OnError(const gchar *msg, const gchar *details);
		
		virtual void OnStop(GstElement *pipeline);
		virtual void OnExit();

	private:

		static void *Run(void *arg);
		
		bool SetState(GstElement *pipeline, GstState state);
		bool WaitForEos(GstElement *pipeline, GstBus *bus);

	private:
		Mutex m_mutex;
		pthread_t m_thread;
		volatile bool m_running;
		std::string m_pipelinestr;
		std::string m_name;
		bool m_restart;
		bool m_exited;
		struct timespec m_restartdelay;
		GstElement *m_pipeline; //For use in local context only
};


