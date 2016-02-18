
class VideoInputSupported
{
	public:
		VideoInputSupported();
		~VideoInputSupported();

		std::vector<std::string> GetCodecs();
		std::vector<std::string> GetCodecResolutions(const std::string &codec);
		std::vector<int> GetCodecFrameRates(const std::string &codec, const std::string &res);

		void AddCodec(const std::string &codec, const std::string &res, int fps);
		void AddCodec(const std::string &codec, const std::string &res, int lowfps, int upperfps);

		int hflip;
		int vflip;

		void Clear();

		std::vector<std::string> ToStrV();
		
		std::string Encode();
		bool Decode(const std::string);
		
	private:
		struct CodecInfo
		{
			std::map<std::string, std::vector<int> > m_res;
			//FIXME: List of modes eg vbr, cbr, cq
			//FIXME: is bitrate, maxbitrate configured
		};

		std::map<std::string, struct CodecInfo> m_codecs;
};

