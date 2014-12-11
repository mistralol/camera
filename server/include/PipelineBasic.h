

class PipelineBasic : public IPipeline
{
	public:
		PipelineBasic(const std::string strpipe);
		~PipelineBasic();

		void Start();
		void Stop();
	private:

		static void *Run(void *arg);

		pthread_t m_thread;
		pthread_mutex_t lock;
		bool m_running;
		std::string m_pipelinestr;
		struct timeval m_restartdelay;
};


