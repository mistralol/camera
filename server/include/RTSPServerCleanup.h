
class RTSPServerCleanup : Thread
{
	public:
		RTSPServerCleanup();
		~RTSPServerCleanup();

		void Init(GstRTSPServer *server);

	private:
		void Run();

		bool m_exit;
		GstRTSPSessionPool *m_pool;
};

