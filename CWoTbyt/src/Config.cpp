#include <fstream>
#include "Config.h"
#include "Log.h"
#include "Utils.h"
#include "INIReader.h"
#include "WOT.h"

#define SETTINGS_FILE             "settings.ini"
#define DEFAULT_BRANCH_NAME       "master"

std::string Config::m_wotPath = "";
std::string Config::m_branch = "";
std::string Config::m_settingsPath = "";
bool Config::m_branchOverride = false;

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
  m_branch = reader.Get("game", "branch", "");

  if (m_wotPath.empty())
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

  if (m_branch.empty())
  {
    m_branch = DEFAULT_BRANCH_NAME;
  }
  else
  {
    m_branchOverride = true;
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

  // THIS IS SO UGLY! It has to be better way...
  // TODO: fix this
  file << "[Game]\n";
  file << "path = " + m_wotPath << "\n";
  file << "# if set, this branch will be used for getting XVM\n";
  file << "# if left empty, the default branch will be used\n";
  file << "branch = " + m_branch << "\n";
  file.close();

  LOG_DEBUG("OK");
}