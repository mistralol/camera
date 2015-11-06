

class PipelineBasic : public IPipeline
{
	public:
		PipelineBasic(const std::string name, const std::string strpipe);
		PipelineBasic(const std::string strpipe);
		~PipelineBasic();

		void SetName(const std::string name);
		void SetRestart(bool value);
		void SetRestartDelay(const struct timespec *ts);

		void Start();
		void Stop();
		bool HasExisted();
		void WaitForExit();
		
		virtual void OnParseError(const gchar *msg);
		virtual void OnStart(GstElement *pipeline);
		virtual void OnStartFailure(GstElement *pipeline);
		
		virtual bool OnBusMessage(GstMessage *msg, bool *stop);
		virtual void OnInfo(const gchar *msg, const gchar *details);
		virtual void OnWarning(const gchar *msg, const gchar *details);
		virtual void OnError(const gchar *msg, const gchar *details);
		
		virtual void OnStop(GstElement *pipeline);
		virtual void OnExit();

	private:
		static void *Run(void *arg);
		
		bool SetState(GstElement *pipeline, GstState state);
		bool WaitForEos(GstElement *pipeline, GstBus *bus);

	protected:
		std::string m_name;

	private:
		Mutex m_mutex;
		pthread_t m_thread;
		volatile bool m_running;
		std::string m_pipelinestr;
		bool m_restart;
		bool m_exited;
		struct timespec m_restartdelay;
		GstElement *m_pipeline; //For use in local context only
};


