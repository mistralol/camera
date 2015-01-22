
class VideoStreamConfig
{
	public:
		VideoStreamConfig();
		~VideoStreamConfig();

		std::string ToString() const;

		bool ConfigSave(Json::Value &json);
		bool ConfigLoad(Json::Value &json);

		int GetFrameRate();
		void SetFrameRate(int fps);

		std::string GetCodec();
		void SetCodec(const std::string &str);

		std::string GetResolution();
		void SetResolution(const std::string &str);

		bool GetEnabled();
		void SetEnabled(bool enabled);

		//FIXME: Codec Options eg bitrate, maxbitrate, vbr/cbr etc..

	private:
		int m_framerate;
		std::string m_codec;
		std::string m_resolution;
		bool m_enabled;

};

