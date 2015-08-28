
class VideoInputSupported
{
	public:
		VideoInputSupported();
		~VideoInputSupported();

		std::list<std::string> GetCodecs();
		std::list<std::string> GetCodecResolutions(const std::string &codec);
		std::list<int> GetCodecFrameRates(const std::string &codec, std::string &res);

		void AddCodec(const std::string &codec, const std::string &res, int fps);
		void AddCodec(const std::string &codec, const std::string &res, int lowfps, int upperfps);

		void Clear();

		void LogDump(); //Dumps everything supported to log file
		
		std::string Encode();
		bool Decode(const std::string);
		
	private:
		struct CodecInfo
		{
			std::map<std::string, std::list<int> > m_res;
			//FIXME: List of modes eg vbr, cbr, cq
			//FIXME: is bitrate, maxbitrate configured
		};

		std::map<std::string, struct CodecInfo> m_codecs;
};

