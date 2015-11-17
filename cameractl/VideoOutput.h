
class VideoOutput
{
	public:
		static void Count(struct Data *data);
		static void Supported(struct Data *data);
		
		static void TourList(struct Data *data);
		static void TourGet(struct Data *data);
		static void TourAdd(struct Data *data);
		static void TourUpdate(struct Data *data);
		static void TourRemove(struct Data *data);
		static void TourExists(struct Data *data);
	
		static void Help(struct Data *data);
		static void Process(struct Data *data);
};

