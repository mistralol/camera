
class RTSPServerCleanup : Thread
{
	public:
		RTSPServerCleanup();
		~RTSPServerCleanup();

		void Init(GstRTSPServer *server);

	private:
		void Run();

		bool m_exit;
		Mutex m_mutex;
		GstRTSPSessionPool *m_pool;
};

