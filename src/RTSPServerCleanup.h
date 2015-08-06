
class RTSPServerCleanup : Thread
{
	public:
		RTSPServerCleanup();
		~RTSPServerCleanup();

		void Start(GstRTSPServer *server);
		void Stop();

	private:
		void Run();
		bool m_started;
		bool m_exit;
		Mutex m_mutex;
		GstRTSPSessionPool *m_pool;
};

