
class VideoOutputConfig
{
	public:
		VideoOutputConfig();
		~VideoOutputConfig();

		std::string ToStr() const;

		bool ConfigLoad(Json::Value &);
		bool ConfigSave(Json::Value &);

	private:

};

