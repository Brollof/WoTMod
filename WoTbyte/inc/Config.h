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

typedef std::vector<Mod> Mods;

class Config
{
public:
  static bool Load();
  static const Xvm& GetXvmData() { return m_xvm; }
  static const Mods& GetModsData() { return m_mods; }
  static void Print();

private:
  Config() {};
  static Xvm m_xvm;
  static Mods m_mods;
};