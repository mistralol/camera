
class Version
{
	public:
		static unsigned int Major();
		static unsigned int Minor();
		static unsigned int Revision();
		static unsigned int Build();
		static std::string ToString();
};

