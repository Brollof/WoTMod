#include <iostream>
#include <codecvt>
#include <fstream>
#include "WotPath.h"
#include "WinReg.hpp"

#define REG_PATH          L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\WOT.EU.PRODUCTION"
#define REG_ITEM          L"InstallLocation"
#define SETTINGS_FILE     "settings.cfg"

std::string* WotPath::GetPathFromReg()
{
  std::cout << "Getting WoT path from registry... ";

  winreg::RegKey key(HKEY_CURRENT_USER, REG_PATH);
  if (!key.IsValid())
  {
    std::cout << "Invalid registery key: " << REG_PATH << std::endl;
    return nullptr;
  }

  std::wstring path;
  winreg::RegResult res = key.TryGetStringValue(REG_ITEM, path);
  if (res.Failed())
  {
    std::wcout << "Can't get registry item: " << REG_ITEM << ", error code: " << res.Code() \
    << " (check winerror.h for more details)" << std::endl;
    return nullptr;
  }

  std::string temp = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(path);
  std::cout << "OK" << std::endl;
  return new std::string(temp);
}

std::string* WotPath::GetPathFromFile()
{
  std::cout << "Getting WoT path from file: " << SETTINGS_FILE << "... ";
  std::ifstream file(SETTINGS_FILE);
  if (!file)
  {
    std::cout << "Can't open file! " << std::endl;
    return nullptr;
  }

  std::string path;
  std::getline(file, path);
  file.close();
  std::cout << "OK" << std::endl;
  return new std::string(path);
}

void WotPath::SavePathToFile(const std::string& path)
{
  std::cout << "Saving WoT path to file: " << SETTINGS_FILE << "... ";
  std::ofstream file(SETTINGS_FILE);
  if (!file)
  {
    std::cout << "Can't open file! " << std::endl;
    return;
  }
  file << path;
  file.close();
  std::cout << "OK" << std::endl;
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