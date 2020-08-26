#pragma once

#include <string>

class Config
{
public:
  static bool Load();
  static const std::string& GetWotPath() { return m_wotPath; }
  static const std::string& GetBranchName() { return m_branch; }
  static bool IsBranchOverriden() { return m_branchOverride; }

private:
  Config() {};
  static void Save();
  static std::string m_wotPath;
  static std::string m_branch;
  static std::string m_settingsPath;
  static bool m_branchOverride;
};