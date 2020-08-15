#include <fstream>
#include "Config.h"
#include "Log.h"
#include "Utils.h"
#include "WinReg.hpp"
#include "INIReader.h"

#define REG_PATH          "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\WOT.EU.PRODUCTION"
#define REG_ITEM          "InstallLocation"
#define SETTINGS_FILE     "settings.ini"
#define XVM_STORAGE       "https://nightly.modxvm.com/download/"
#define XVM_FILENAME      "xvm_latest.zip"

// full link example: https://nightly.modxvm.com/download/master/xvm_latest.zip

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

  m_wotPath = reader.Get("game", "path", "");
  m_branch = reader.Get("game", "branch", "master");

  if (Config::m_wotPath.empty())
  {
    const std::string* path = Config::GetPathFromReg();
    if (!path)
    {
      LOG_ERROR("WoT path read failed!");
      return false;
    }
    m_wotPath = *path;
    delete path;
    Config::Save();
  }

  LOG_DEBUG("Cfg wot path: {}", m_wotPath);
  LOG_DEBUG("Cfg branch name: {}", m_branch);
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
  file << "path = " + m_wotPath << std::endl;
  file << "branch = " + m_branch << std::endl;
  file.close();

  LOG_DEBUG("OK");
}

const std::string Config::GetXvmUrl()
{
  return XVM_STORAGE + m_branch + "/" + XVM_FILENAME;
}