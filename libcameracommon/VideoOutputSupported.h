
class VideoOutputSupported
{
	public:
		VideoOutputSupported();
		~VideoOutputSupported();
		
		void SetMaxPanes(int );
		int GetMaxPanes();
		
		void Clear();
		
		std::string Encode();
                bool Decode(const std::string);
                
	private:
		int m_maxpanes;
};

