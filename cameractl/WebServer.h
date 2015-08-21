
class WebServer
{
	public:
		static int Enabled(struct Data *data);
		static int Restart(struct Data *data);
		static int Port(struct Data *data);
	
		static int Help(struct Data *data);
		static int Process(struct Data *data);
};

