
class Rtsp
{
	public:
		static int MaxBacklog(struct Data *data);
		static int MaxClients(struct Data *data);
		static int Port(struct Data *data);
		static int KickAll(struct Data *data);
		static int Count(struct Data *data);

		static int Help(struct Data *data);
		static int Process(struct Data *data);
};

