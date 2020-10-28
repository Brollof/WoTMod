#pragma once

#include <string>

class WOT
{
public:
  static const std::string& GetPath() { return m_wotPath; }
  static const std::string& GetVersion() { return m_wotVersion; }
  static const std::string& GetPathFromReg();
  static const std::string& ExtractWotVersion(std::string wotPath);

private:
  WOT() {};
  static std::string m_wotPath;
  static std::string m_wotVersion;
};