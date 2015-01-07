
class VideoStreamConfig
{
	public:
		VideoStreamConfig();
		~VideoStreamConfig();

		int GetFrameRate();
		void SetFrameRate(int fps);

		std::string GetCodec();
		void SetCodec(const std::string &str);

		std::string GetResolution();
		void SetResolutions(const std::string &str);

		//FIXME: Codec Options eg bitrate, maxbitrate, vbr/cbr etc..

	private:
		int m_framerate;
		std::string m_codec;
		std::string m_resolution;

};

