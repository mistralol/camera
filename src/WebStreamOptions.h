

class WebStreamOptions
{
	public:
		WebStreamOptions();

		void Encode(Json::Value &options);
		void Decode(const Json::Value &options);
		
		int vinput;
		WebStreamType type;
		bool localonly;
		int timeout;

		/* Transcoding */
		int width;
		int height;

		/* MJPEG TransCode */
		int mjpeg_fps;
		int mjpeg_quality;
		
};


