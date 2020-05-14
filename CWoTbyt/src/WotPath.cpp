#include <fstream>
#include "WotPath.h"
#include "WinReg.hpp"
#include "Log.h"
#include "Utils.h"

#define REG_PATH          "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\WOT.EU.PRODUCTION"
#define REG_ITEM          "InstallLocation"
#define SETTINGS_FILE     "settings.cfg"

std::string* WotPath::GetPathFromReg()
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

std::string* WotPath::GetPathFromFile()
{
  LOG_DEBUG("Getting WoT path from file: {}... ", SETTINGS_FILE);
  std::ifstream file(SETTINGS_FILE);
  if (!file)
  {
    LOG_DEBUG("Can't open file!");
    return nullptr;
  }

  std::string path;
  std::getline(file, path);
  file.close();
  LOG_DEBUG("OK");
  return new std::string(path);
}

void WotPath::SavePathToFile(const std::string& path)
{
  LOG_DEBUG("Saving WoT path to file: {}... ", SETTINGS_FILE);
  std::ofstream file(SETTINGS_FILE);
  if (!file)
  {
    LOG_DEBUG("Can't open file! ");
    return;
  }
  file << path;
  file.close();
  LOG_DEBUG("OK");
}

std::string* WotPath::GetInstallPath()
{
  std::string* path = WotPath::GetPathFromFile();
  if (path)
  {
    return path;
  }

  path = WotPath::GetPathFromReg();
  if (path)
  {
    WotPath::SavePathToFile(*path);
    return path;
  }

  return nullptr;
}