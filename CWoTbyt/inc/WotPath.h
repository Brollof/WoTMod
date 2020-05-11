#pragma once

#include <string>

class WotPath
{
public:
  static std::string* GetInstallPath();

private:
  WotPath();
  static std::string* GetPathFromReg();
  static std::string* GetPathFromFile();
  static void SavePathToFile(const std::string& path);
};