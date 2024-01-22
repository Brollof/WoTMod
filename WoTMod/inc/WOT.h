#pragma once

#include <string>
#include "Config.h"

class WOT
{
public:
  static const std::string GetVersion();
  static const std::string& GetPath();
  static const std::string ExtractWotVersion(std::string wotPath);

private:
  WOT() {};
  static std::string m_wotPath;
};