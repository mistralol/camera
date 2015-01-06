
#include <Camera.h>

Config::Config(CameraHandler *Handler, const std::string &FName)
{
	m_handler = Handler;
	m_CfgFile = FName;
}

Config::~Config()
{

}

bool Config::Load()
{
	LogDebug("Config::Load");
	std::string json = "";
	Json::Value root;   // will contains the root value after parsing.
	Json::Reader reader;
	bool HaveFile = false;

	if (FileUtil::Exists(m_CfgFile))
	{
		int err = FileUtil::Read(m_CfgFile, &json);
		if (err < 0)
		{
			LogCritical("CameraHandler::ConfigLoad: Cannot read config file \"%s\" error: %s", m_CfgFile.c_str(), Errno::ToStr(err).c_str());
			abort();
		}
		HaveFile = true;
	}
	else //Try backup file
	{
		std::string BakFile = m_CfgFile + ".bak";
		LogInfo("CameraHandler::ConfigLoad: No Config File Found. Trying Backup: \"%s\"", BakFile.c_str());
		if (FileUtil::Exists(BakFile))
		{
			int err = FileUtil::Read(BakFile, &json);
			if (err < 0)
			{
				LogCritical("CameraHandler::ConfigLoad: Cannot read config file \"%s\" error: %s", BakFile.c_str(), Errno::ToStr(err).c_str());
				abort();
			}
			HaveFile = true;
		}
		else
		{
			LogInfo("CameraHandler::ConfigLoad: No Config File Found. Using Defaults");
		}
	}

	if (HaveFile)
	{
		if (reader.parse(json, root ) == false)
		{
			LogCritical("CameraHandler::ConfigLoad: Could not parse json: \"%s\"", json.c_str());
			abort();
		}
	}

	m_handler->ConfigLoad(root);

	//If we dont have a config file. Make a new config file
	if (HaveFile == false)
		Dirty();

	return true;
}

bool Config::Save()
{
	LogDebug("Config::Save");
	std::string BakFile = m_CfgFile + ".bak";
	Json::Value json;

	if (m_handler->ConfigSave(json) == false)
	{
		LogError("Cannot Write Config: CameraHandler::ConfigSave Failed!");
		return false;
	}

	if (FileUtil::Exists(BakFile))
	{
		if (unlink(BakFile.c_str()) < 0)
		{
			LogCritical("Config::Save - Cannot unlink backup file \"%s\" error: %s", BakFile.c_str(), Errno::ToStr(errno).c_str());
			abort();
		}
	}

	if (FileUtil::Exists(m_CfgFile))
	{
		if (rename(m_CfgFile.c_str(), BakFile.c_str()) < 0)
		{
			LogCritical("Config::Save - Cannot rename \"%s\" -> \"%s\" error: %s", m_CfgFile.c_str(), BakFile.c_str(), Errno::ToStr(errno).c_str());
			abort();
		}
	}

	//Write new config file
	{
		std::stringstream ss;
		Json::StyledWriter styledWriter;
		ss << styledWriter.write(json);
		int err = FileUtil::WriteSync(m_CfgFile.c_str(), ss.str());
		if (err < 0)
		{
			LogCritical("Config::Save - Cannot write config file \"%s\" error: %s", m_CfgFile.c_str(), Errno::ToStr(errno).c_str());
		}
	}
	return true;
}

void Config::Dirty()
{
	//TODO: for now just call save. However we could do this async from a background thread
	Save();
}


