#pragma once

#include <string>

class WOT
{
public:
  static const std::string GetVersion();
  static const std::string& GetPath();
  static const std::string ExtractWotVersion(std::string wotPath);

private:
  WOT() {};
  static const std::string GetPathFromReg();
  static std::string m_wotPath;
};