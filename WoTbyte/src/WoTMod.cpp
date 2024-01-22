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

static bool DownloadAndExtract(std::string src, std::string dst)
{
#if (SKIP_DOWNLOAD == 0)
  TempDownloader dl;
  if (dl.Download(src) == false)
  {
    return false;
  }
#else
  LOG_DEBUG("Download skipped");
#endif

#if (SKIP_EXTRACTION == 0)
  Extractor extractor;
  return extractor.Extract(dl.GetFilePath(), dst);
#else
  LOG_DEBUG("Extraction skipped");
#endif
  return true;
}

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

  // Debug only
  Config::Print();

  // Setup XVM
  if (DownloadAndExtract(XVM::GetFileUrl(), WOT::GetPath()) == false)
  {
    AppEnd();
    return 0;
  }

  // Setup mods
  for (auto mod : Config::GetModsData())
  {
    LOG_INFO("Installig mod: {}", mod.Name);
    DownloadAndExtract(mod.Url, WOT::GetPath());
  }

  LOG_INFO("All mods installed!\n");
  AppEnd();
  return 0;
}