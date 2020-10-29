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

  Xvm xvm = Config::GetXvmData();
  LOG_DEBUG("cfg - branch: {}", xvm.Branch);
  LOG_DEBUG("cfg - filename: {}", xvm.Filename);
  LOG_DEBUG("cfg - branches url: {}", xvm.UrlBranches);
  LOG_DEBUG("cfg - storage url: {}", xvm.UrlStorage);

  for (auto mod : Config::GetModsData())
  {
    LOG_DEBUG("cfg - mod - name: {}", mod.Name);
    LOG_DEBUG("cfg - mod - url: {}", mod.Url);
  }

  // Setup XVM
  std::string xvmUrl = XVM::GetFileUrl();
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
  Extractor zipEx = Extractor();
  zipEx.Extract(dl.GetFilePath(), WOT::GetPath());
#endif

  for (auto mod : Config::GetModsData())
  {
    LOG_INFO("Installig mod: {}", mod.Name);
    TempDownloader dl;
    if (dl.Download(mod.Url) == false)
    {
      continue;
    }
    zipEx.Extract(dl.GetFilePath(), WOT::GetPath());
  }

  LOG_INFO("All mods installed!\n");
  AppEnd();
  return 0;
}