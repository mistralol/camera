
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

	private:
		std::string m_source;
		int m_delay;

};

