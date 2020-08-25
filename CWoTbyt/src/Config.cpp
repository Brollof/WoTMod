#include <fstream>
#include "Config.h"
#include "Log.h"
#include "Utils.h"
#include "INIReader.h"
#include "WOT.h"

#define SETTINGS_FILE     "settings.ini"

// full link example: https://nightly.modxvm.com/download/master/xvm_latest.zip

std::string Config::m_wotPath = "";
std::string Config::m_branch = "";
std::string Config::m_settingsPath = "";

bool Config::Load()
{
  m_settingsPath = FullPath(SETTINGS_FILE);
  LOG_DEBUG("Loading config from {}...", m_settingsPath);

  INIReader reader(m_settingsPath);
  if (reader.ParseError() != 0)
  {
    LOG_ERROR("Can't load config file: {}", m_settingsPath);
    return false;
  }

  m_wotPath = reader.Get("game", "path", "");
  m_branch = reader.Get("game", "branch", "master");

  if (Config::m_wotPath.empty())
  {
    const std::string& path = WOT::GetPathFromReg();
    if (path.empty())
    {
      LOG_ERROR("WoT path read failed!");
      return false;
    }
    m_wotPath = path;
    Config::Save();
  }

  LOG_DEBUG("Cfg wot path: {}", m_wotPath);
  LOG_DEBUG("Cfg branch name: {}", m_branch);
  return true;
}

void Config::Save()
{
  LOG_DEBUG("Saving WoT path to file: {}... ", m_settingsPath);

  std::ofstream file(m_settingsPath);
  if (!file)
  {
    LOG_DEBUG("Can't open file! ");
    return;
  }

  file << "[Game]" << std::endl;
  file << "path = " + m_wotPath << std::endl;
  file << "branch = " + m_branch << std::endl;
  file.close();

  LOG_DEBUG("OK");
}