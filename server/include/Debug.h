


class Debug
{
	public:
	
		static bool ConfigLoad(Json::Value &json);
		static bool ConfigSave(Json::Value &json);
	
		static void SetEnabled(bool enabled);
		static bool GetEnabled();
		
	private:
		static bool m_enabled;
};

bool IsDebug(); //Global Function for Easy Access

