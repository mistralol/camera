
struct VideoStreamCodecInfo
{
	std::string Resolution;		//Resolution Supported Could be XxY format or D1, 2CIF etc...
	std::list<int> FrameRates;	//List of frame rates to be supported at resolutions

	void Init()
	{
		Resolution = "";
		FrameRates.clear();
	}
};

struct VideoStreamCodecModeInfo
{
	std::string Mode;	//Encoding Mode e VBR / CBR / CQ / etc...

	//These options tell us which settings to use for the specific mode.
	bool BitRate;
	bool MaxBitRate;
	bool Quality;

	bool GOPSize;

	void Init()
	{
		BitRate = false;
		MaxBitRate = false;
		Quality = false;
		GOPSize = false;
	}
};

struct VideoStreamCodecSupported
{
	std::string CodecName;				//Codec Name eg MJPEG / H264 / etc...
	std::list<struct VideoStreamCodecInfo> Info;	//Supported Frame Rates and Resoltuions for this codec.

	//If param is empty assume no mode are supported
	std::map<std::string, struct VideoStreamCodecModeInfo> Param;	//List of Encoding Modes and supported paramaters

	void Init()
	{
		CodecName = "";
		Info.clear();
		Param.clear();
	}
};

struct VideoStreamSupported
{
	std::map<std::string, struct VideoStreamCodecSupported> Codecs;	//<CodecName, Struct>

	void Init()
	{
		Codecs.clear();
	}

	void AddCodec(const struct VideoStreamCodecSupported *Codec)
	{
		Codecs[Codec->CodecName] = *Codec;
	}

};

struct VideoSupported
{
	unsigned int NumStreams;		//Number of Video Streams Supported
	std::map<unsigned int, struct VideoStreamSupported> Stream;	//Info For Each Stream

	void Init()
	{
		NumStreams = 0;
		Stream.clear();
	}
};



