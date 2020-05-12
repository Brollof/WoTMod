#ifdef _DEBUG
  #define FAST_URL 0
  #define SKIP_DOWNLOAD 1
  #define SKIP_EXTRACTION 1
#endif

#include <iostream>
#include "Common.h"
#include "TempDownloader.h"
#include "ZipExtractor.h"
#include "WotPath.h"
#include "Log.h"

int main()
{
  Log::Init();

#if (SKIP_DOWNLOAD == 0)
  TempDownloader dl = TempDownloader(XVM_URL);
  if (dl.Download() == false)
  {
    return 0;
  }
#endif

  std::string* wotPath = WotPath::GetInstallPath();
  if (!wotPath)
  {
    LOG_ERROR("WoT path read failed!");
    return 0;
  }

  std::cout << "Wot path obtained: " << *wotPath << std::endl;
#if (SKIP_EXTRACTION == 0)
  ZipExtractor ex;
  ex.Extract(dl.GetFilePath(), *wotPath);
#endif

  delete wotPath;

  std::cout << "Done." << std::endl;
}