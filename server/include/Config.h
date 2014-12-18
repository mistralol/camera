
class CameraHandler;

class Config
{
	public:
		Config(CameraHandler *Handler, const std::string &FName);
		~Config();
		bool Load();
		bool Save();	//Do not call this direct to save the config use Dirty() instead
		void Dirty();	//Use Dirty to perform a delayed write of the config file

	private:
		CameraHandler *m_handler;
		std::string m_CfgFile;
};


