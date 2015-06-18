
class WebServerProcess : protected Thread
{
	public:
		WebServerProcess();
		~WebServerProcess();
		
		void Start();
		void Stop();
		
	private:
		void Run();
};

