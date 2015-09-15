

class WebStream
{
	public:
		WebStream();
		~WebStream();
	
		int StartVideoInput(WebStreamOptions *); /* Returns port number to connect to */
		static void Cleanup(void *arg);

	private:
		TimerFunc *m_TmrCleanup;
		std::list<WebStreamPipeline *> m_pipelines;
		Mutex m_mutex;
};



