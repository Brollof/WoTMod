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
  std::string branch = Config::GetBranchName();

  LOG_DEBUG("Cfg wot path: {}", wotPath);
  LOG_DEBUG("Cfg branch name: {}", branch);

  if (wotPath.empty())
  {
    LOG_ERROR("WoT path read failed!");
    AppEnd();
    return 0;
  }

#if (SKIP_DOWNLOAD == 0)
  TempDownloader dl = TempDownloader(XVM_URL);
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