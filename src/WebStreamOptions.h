

class WebStreamOptions
{
	public:
		WebStreamOptions();

		bool Encode(Json::Value &str);
		bool Decode(const Json::Value &str);
		
		int vinput;
		std::string type;
		bool localonly;
		int timeout;
};


