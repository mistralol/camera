
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
		std::string m_strpipe;
		volatile bool m_finished;
		int m_fd;

};

