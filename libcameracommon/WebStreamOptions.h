

class WebStreamOptions
{
	public:
		WebStreamOptions();

		bool Encode(std::string &str);
		bool Decode(const std::string &str);
		
		int vinput;
		WebStreamType type;
		bool localonly;
		int timeout;
		
};


