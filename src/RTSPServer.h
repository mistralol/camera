
class RTSPServer : Thread
{
	public:
		RTSPServer();
		~RTSPServer();

		void VideoPipelineAdd(const std::string url, const std::string pipeline);
		void VideoPipelineRemove(const std::string url);

		void KickAll();

		int SetPort(int port);
		int GetPort();

		void SessionsSetMax(guint max);
		guint SessionsGetMax();
		guint SessionsCount();

		void BacklogSet(guint max);
		guint BacklogGet();

		bool ConfigLoad(Json::Value &);
		bool ConfigSave(Json::Value &);

		//Get/Set Port
		//Set/Set Address
		//Multicast?

	private:
		void Run();

		int m_port;
		GMainLoop *m_loop;
		GstRTSPServer *m_server;
		GstRTSPAuth *m_auth;
		Barrier m_startbar;
		Mutex m_mutex;
		std::map<std::string, std::string> m_videopipelines;
};


