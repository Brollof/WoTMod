#include "Config.h"
#include "Log.h"
#include "WOT.h"
#include "pugixml.hpp"
#include <filesystem>

#define CONFIG_FILE             "config.xml"

Xvm Config::m_xvm;
Mods Config::m_mods;
std::string Config::m_wotPath;

bool Config::Load()
{
  std::string cfgPath = FullPath(CONFIG_FILE);
  LOG_DEBUG("Loading config from '{}'...", cfgPath);
  
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file("config.xml");
  if (!result)
  {
    LOG_DEBUG("Can't load config file!");
    return false;
  }

  // Get WoT installation path
  m_wotPath = doc.child("config").child("game_path").text().as_string();
  if (std::filesystem::exists(m_wotPath) == false)
  {
    LOG_ERROR("World of Tank path: '{}' doesn't exist!", m_wotPath);
    return false;
  }

  // Get XVM data
  auto xvmCfg = doc.child("config").child("xvm");
  m_xvm.Branch = xvmCfg.child("branch").text().as_string();
  m_xvm.UrlBranches = xvmCfg.child("url_branches").text().as_string();
  m_xvm.UrlStorage = xvmCfg.child("url_storage").text().as_string();
  m_xvm.Filename = xvmCfg.child("filename").text().as_string();

  // Get mods data
  for (pugi::xml_node xmlMod : doc.child("config").child("mods").children("mod"))
  {
    Mod mod;
    mod.Name = xmlMod.child("name").text().as_string();
    mod.Url = xmlMod.child("url").text().as_string();

    LOG_DEBUG("Mod name: {}", mod.Name);
    LOG_DEBUG("Mod url: {}", mod.Url);

    m_mods.push_back(mod);
  }

  return true;
}

void Config::Print()
{
  LOG_DEBUG("cfg - game path: {}", m_wotPath);
  LOG_DEBUG("cfg - branch: {}", m_xvm.Branch);
  LOG_DEBUG("cfg - filename: {}", m_xvm.Filename);
  LOG_DEBUG("cfg - branches url: {}", m_xvm.UrlBranches);
  LOG_DEBUG("cfg - storage url: {}", m_xvm.UrlStorage);

  for (auto mod : m_mods)
  {
    LOG_DEBUG("------------------------------");
    LOG_DEBUG("cfg - mod - name: {}", mod.Name);
    LOG_DEBUG("cfg - mod - url: {}", mod.Url);
    LOG_DEBUG("------------------------------");
  }
}
