
class VideoOutputSupported
{
	public:
		VideoOutputSupported();
		~VideoOutputSupported();
		
		void Clear();
		
		std::string Encode();
                bool Decode(const std::string);
                
	private:
		int m_maxpanes;
};

