
class VideoOutputTour
{
	public:
		VideoOutputTour();
		VideoOutputTour(const std::string name);
		~VideoOutputTour();
		
		std::string GetName();
		void SetName(const std::string name);
		
		void AddSource(const VideoOutputTourItem &item);
		void AddSource(const std::string source, int delay);
		void Clear();
		
		//Encode
		//ConfigSave
		
	private:
		std::string m_name;
		std::list<VideoOutputTourItem> m_items;
		
};

