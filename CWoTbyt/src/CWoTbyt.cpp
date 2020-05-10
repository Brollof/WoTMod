#include <iostream>
#include "Common.h"
#include "TempDownloader.h"

#define SKIP_DOWNLOAD

int main()
{
#ifndef SKIP_DOWNLOAD
	TempDownloader dl = TempDownloader(XVM_URL);
	if (dl.Download())
	{
		return 1;
	}
#endif


	
}
