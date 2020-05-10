#ifdef _DEBUG
  #define FAST_URL 1
  #define SKIP_DOWNLOAD 0
  #define SKIP_EXTRACTION 1
#endif

#include <iostream>
#include "Common.h"
#include "TempDownloader.h"
#include "ZipExtractor.h"

int main()
{
#if (SKIP_DOWNLOAD == 0)
  TempDownloader dl = TempDownloader(XVM_URL);
  if (dl.Download() == false)
  {
    return 0;
  }
#endif


#if (SKIP_EXTRACTION == 0)
  ZipExtractor ex;
  ex.Extract(zip_file, out_dir);
#endif

}
