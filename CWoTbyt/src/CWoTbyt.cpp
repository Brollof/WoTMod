#ifdef _DEBUG
  #define SKIP_DOWNLOAD 1
  #define SKIP_EXTRACTION 1
#endif

#include "Common.h"
#include "TempDownloader.h"
#include "ZipExtractor.h"
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
 
  if (http.IsInitialized())
  {
    LOG_DEBUG("HTTP module initialized");
    std::string data = http.Get("https://nightly.modxvm.com/");
    xvmUrl = XVM::GetUrl(wotPath, data);
  }
  
  if (xvmUrl.empty()) // fallback -> build url according to branch name in config file
  {
    LOG_DEBUG("XVM branch fallback -> getting from config file");
    xvmUrl = XVM::GetUrl(Config::GetBranchName());
  }

  LOG_DEBUG("XVM url: {}", xvmUrl);

#if (SKIP_DOWNLOAD == 0)
  TempDownloader dl = TempDownloader(xvmUrl);
  if (dl.Download() == false)
  {
    AppEnd();
    return 0;
  }
#endif

#if (SKIP_EXTRACTION == 0)
  ZipExtractor ex;
  ex.Extract(dl.GetFilePath(), wotPath);
#endif

  LOG_INFO("XVM updated successfully!\n"); // \n for extra spacing
  AppEnd();
  return 0;
}