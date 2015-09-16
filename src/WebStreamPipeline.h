
class WebStreamPipeline : private Thread
{
	public:
		WebStreamPipeline(WebStreamOptions *);
		~WebStreamPipeline();
	
		int Start();
		void Stop();

		bool HasFinished();
	private:
		void Run();
	
	
	private:
		WebStreamOptions m_options;
		volatile bool m_finished;
		volatile bool m_running;
		int m_fd;

};

