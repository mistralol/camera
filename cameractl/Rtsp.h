
class Rtsp
{
	public:
		static void MaxBacklog(struct Data *data);
		static void MaxClients(struct Data *data);
		static void Port(struct Data *data);
		static void KickAll(struct Data *data);
		static void Count(struct Data *data);

		static void Help(struct Data *data);
		static void Process(struct Data *data);
};

