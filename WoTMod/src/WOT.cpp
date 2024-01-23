#include "WOT.h"
#include "Log.h"
#include "WinReg.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <regex>

#define WOT_VERSION_FILE  "version.xml"

std::string WOT::m_wotPath = "";

const std::string WOT::ExtractWotVersion(std::string wotPath)
{
  std::string versionFilePath = (std::filesystem::path(wotPath) / WOT_VERSION_FILE).string();
  LOG_DEBUG("WoT version file path: {}", versionFilePath);

  std::ifstream file(versionFilePath);
  if (!file.is_open())
  {
    LOG_DEBUG("Can't open file! {}", versionFilePath);
    return "";
  }
  
  std::stringstream buffer;
  buffer << file.rdbuf();

  std::regex exp("<version>.*?(\\d+\\.\\d+\\.\\d+\\.\\d+).*?</version>");
  std::smatch match;
  std::string data = buffer.str();
  std::string wotVersion = "";

  if (std::regex_search(data, match, exp))
  {
    LOG_DEBUG("regex_search for wot version succeeded");
    wotVersion = match[1];
  }
  else
  {
    LOG_DEBUG("regex_search for wot version failed!");
  }

  LOG_DEBUG("wot version: {}", wotVersion);
  file.close();
  return wotVersion;
}

const std::string WOT::GetVersion()
{
  std::string wotPath = GetPath();
  return ExtractWotVersion(wotPath);
}

const std::string& WOT::GetPath()
{
  return Config::GetWotPath();
}
