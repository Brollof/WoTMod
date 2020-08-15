#include <fstream>
#include "Config.h"
#include "Log.h"
#include "Utils.h"
#include "WinReg.hpp"
#include "INIReader.h"

#define REG_PATH          "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\WOT.EU.PRODUCTION"
#define REG_ITEM          "InstallLocation"
#define SETTINGS_FILE     "settings.ini"

std::string Config::m_wotPath = "";
std::string Config::m_branch = "";

const std::string* Config::GetPathFromReg()
{
  LOG_DEBUG("Getting WoT path from registry... ");

  winreg::RegKey key(HKEY_CURRENT_USER, STR_TO_WSTR(REG_PATH));
  if (!key.IsValid())
  {
    LOG_DEBUG("Invalid registry key: {}", REG_PATH);
    return nullptr;
  }

  std::wstring path;
  winreg::RegResult res = key.TryGetStringValue(STR_TO_WSTR(REG_ITEM), path);
  if (res.Failed())
  {
    LOG_DEBUG("Can't get registry item: {}, error code: {} (check winerror.h for more details)",
      REG_ITEM, res.Code());
    return nullptr;
  }

  LOG_DEBUG("OK");
  return new std::string(WSTR_TO_STR(path));
}

bool Config::Load()
{
  LOG_DEBUG("Loading config from {}...", SETTINGS_FILE);

  INIReader reader(SETTINGS_FILE);
  if (reader.ParseError() != 0)
  {
    LOG_ERROR("Can't load config file: {}", SETTINGS_FILE);
    return false;
  }

  Config::m_wotPath = reader.Get("game", "path", "");
  Config::m_branch = reader.Get("game", "branch", "master");

  if (Config::m_wotPath.empty())
  {
    const std::string* path = Config::GetPathFromReg();
    if (!path)
    {
      LOG_ERROR("WoT path read failed!");
      return false;
    }
    Config::m_wotPath = *path;
    delete path;
    Config::Save();
  }

  LOG_DEBUG("OK");
  return true;
}

void Config::Save()
{
  LOG_DEBUG("Saving WoT path to file: {}... ", SETTINGS_FILE);

  std::ofstream file(SETTINGS_FILE);
  if (!file)
  {
    LOG_DEBUG("Can't open file! ");
    return;
  }

  file << "[Game]" << std::endl;
  file << "path = " + Config::m_wotPath << std::endl;
  file << "branch = " + Config::m_branch << std::endl;
  file.close();

  LOG_DEBUG("OK");
}