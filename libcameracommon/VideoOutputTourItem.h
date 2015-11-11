
class VideoOutputTourItem
{
	public:
		VideoOutputTourItem();
		VideoOutputTourItem(const std::string src, int delay);
		~VideoOutputTourItem();

		std::string GetSource();
		void SetSource(const std::string source);

		int GetDelay();
		void SetDelay(int seconds);

		bool ConfigSave(Json::Value &json);
		bool ConfigLoad(Json::Value &json);
		
	private:
		std::string m_source;
		int m_delay;
};

