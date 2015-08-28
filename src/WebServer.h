
class WebServer : private Thread
{
	public:
		WebServer(const std::string root);
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
		
		std::string GetProperty(const std::string key);
		std::string GetProperty(const std::string key, const std::string def);
		void SetProperty(const std::string key, const std::string value);
		
	private:
		bool Exec(); //Start sub process
		void Run(); //Monitoring Thread
	
		Mutex m_mutex;
		bool m_enabled;
		volatile bool m_running;
		int m_port;
		pid_t m_pid;
		std::map<std::string, std::string> m_props;
		std::string m_WebRoot;
		
};
