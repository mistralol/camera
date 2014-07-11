
class RTSPHandler : public IServerHandler
{
	public:
		RTSPHandler();
		~RTSPHandler();

		void OnPreNewConnection();

		void OnPostNewConnection(IServerConnection *Connection);		
		void OnDisconnect(IServerConnection *Connection);
		
		int OnRequest(IServerConnection *Connection, Request *request, Request *response);

		int OnCommand(IServerConnection *Connection, Request *request);
		void OnBadLine(IServerConnection *Connection, const std::string *line);

		void Wait();
		void Quit();

	private:
		Barrier m_QuitBarrier;

};


