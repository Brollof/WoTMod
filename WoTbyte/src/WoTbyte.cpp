#ifdef _DEBUG
  #define SKIP_DOWNLOAD 1
  #define SKIP_EXTRACTION 1
#endif

#include "Common.h"
#include "TempDownloader.h"
#include "Extractor.h"
#include "Log.h"
#include "Version.h"
#include "Utils.h"
#include "Config.h"
#include "Http.h"
#include "XVM.h"
#include "WOT.h"

int main()
{
  Log::Init();
  PrintVersion();
  PrintBuildType();

  if (!Config::Load())
  {
    AppEnd();
    return 0;
  }

  std::string wotPath = Config::GetWotPath();
  if (wotPath.empty())
  {
    LOG_ERROR("WoT path read failed!");
    AppEnd();
    return 0;
  }
 
  Http http;
  std::string xvmUrl;
 
  // Branch selection algorithm
  // 1. Check if branch is overriden - if so, use it, otherwise proceed to p.2
  // 2. Try to fetch branch from xvm website - otherwise proceed to p.3
  // 3. Fallback - get default branch

  if (Config::IsBranchOverriden())
  {
    LOG_DEBUG("Branch is overriden to: {}", Config::GetBranchName());
    xvmUrl = XVM::GetUrl(Config::GetBranchName());
  }
  else
  {
    LOG_DEBUG("HTTP module initialized");
    std::string data = http.Get("https://nightly.modxvm.com/");
    xvmUrl = XVM::GetUrl(wotPath, data);
  }

  // Fallback - if everything else failed use default branch
  if (xvmUrl.empty())
  {
    LOG_DEBUG("Using default branch");
    xvmUrl = XVM::GetUrl();
  }

  LOG_DEBUG("XVM url: {}", xvmUrl);

#if (SKIP_DOWNLOAD == 0)
  TempDownloader dl;
  if (dl.Download(xvmUrl) == false)
  {
    AppEnd();
    return 0;
  }
#endif

#if (SKIP_EXTRACTION == 0)
  Extractor zipEx = Extractor(Extractor::Format::ZIP);
  zipEx.Extract(dl.GetFilePath(), wotPath);
#endif

  LOG_INFO("XVM updated successfully!\n"); // \n for extra spacing

#if (SKIP_DOWNLOAD == 0)
  if (Config::GetMoeMod() == true)
  {
    LOG_INFO("Installing MoE mod...");
    Extractor rarEx = Extractor(Extractor::Format::RAR);
    dl.Download("http://down.wotspeak.org/756-mod_marksOnGunExtended.rar");
    rarEx.Extract(dl.GetFilePath(), wotPath);
  }
#endif

  LOG_INFO("All mods installed!\n");
  AppEnd();
  return 0;
}