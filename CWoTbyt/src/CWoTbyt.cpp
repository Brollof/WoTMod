#ifdef _DEBUG
  #define FAST_URL 1
  #define SKIP_DOWNLOAD 0
#endif

#include <iostream>
#include "Common.h"
#include "TempDownloader.h"


int main()
{
#if (SKIP_DOWNLOAD == 0)
  TempDownloader dl = TempDownloader(XVM_URL);
  if (dl.Download() == false)
  {
    return 0;
  }
#endif



}
