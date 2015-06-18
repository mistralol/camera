
class WebServer : private Thread
{
	public:
		WebServer();
		~WebServer();
		
		bool ConfigLoad(Json::Value &);
		bool ConfigSave(Json::Value &);
		
		void Start();
		void Stop();
		void Restart();
		
		void SetEnabled(bool enabled);
		bool GetEnabled();
		
		int SetPort(int port);
		int GetPort();
		
	private:
		void Run(); //Monitoring Thread
	
		Mutex m_mutex;
		bool m_enabled;
		int m_port;
		pid_t m_pid;
		
};
