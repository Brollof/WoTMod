#pragma once

#include <string>
#include <vector>

struct Mod
{
  std::string Name;
  std::string Url;
};

struct Xvm
{
  std::string Branch;
  std::string UrlBranches;
  std::string UrlStorage;
  std::string Filename;
};

using Mods = std::vector<Mod>;

class Config
{
public:
  static bool Load();
  static const Xvm& GetXvmData() { return m_xvm; }
  static const Mods& GetModsData() { return m_mods; }
  static const std::string& GetWotPath() { return m_wotPath; }
  static void Print();

private:
  Config() {};
  static Xvm m_xvm;
  static Mods m_mods;
  static std::string m_wotPath;
};