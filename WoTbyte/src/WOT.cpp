#include "WOT.h"
#include "Log.h"
#include "WinReg.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <regex>

#define REG_PATH          "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\WOT.EU.PRODUCTION"
#define REG_ITEM          "InstallLocation"
#define WOT_VERSION_FILE  "version.xml"

std::string WOT::m_wotPath = "";
std::string WOT::m_wotVersion = "";

const std::string& WOT::GetPathFromReg()
{
  LOG_DEBUG("Getting WoT path from registry... ");

  winreg::RegKey key(HKEY_CURRENT_USER, STR_TO_WSTR(REG_PATH));
  if (!key.IsValid())
  {
    LOG_DEBUG("Invalid registry key: {}", REG_PATH);
    return m_wotPath;
  }

  std::wstring path;
  winreg::RegResult res = key.TryGetStringValue(STR_TO_WSTR(REG_ITEM), path);
  if (res.Failed())
  {
    LOG_DEBUG("Can't get registry item: {}, error code: {} (check winerror.h for more details)",
      REG_ITEM, res.Code());
    return m_wotPath;
  }

  LOG_DEBUG("OK");
  m_wotPath = std::string(WSTR_TO_STR(path));
  return m_wotPath;
}

const std::string& WOT::ExtractWotVersion(std::string wotPath)
{
  std::string versionFilePath = (std::filesystem::path(wotPath) / WOT_VERSION_FILE).string();
  LOG_DEBUG("WoT version file path: {}", versionFilePath);

  std::ifstream file(versionFilePath);
  if (!file.is_open())
  {
    LOG_DEBUG("Can't open file! {}", versionFilePath);
    return m_wotVersion;
  }
  
  std::stringstream buffer;
  buffer << file.rdbuf();

  std::regex exp("<version>.*?(\\d+\\.\\d+\\.\\d+\\.\\d+).*?</version>");
  std::smatch match;
  std::string data = buffer.str();

  if (std::regex_search(data, match, exp))
  {
    LOG_DEBUG("regex_search for wot version succeeded");
    m_wotVersion = match[1];
  }
  else
  {
    LOG_DEBUG("regex_search for wot version failed!");
  }

  LOG_DEBUG("wot version: {}", m_wotVersion);
  file.close();
  return m_wotVersion;
}