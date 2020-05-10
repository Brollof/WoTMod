// CWoTbyt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "TempDownloader.h"

#define FAST_URL 1

#define TEST_URL "https://www.w3.org/TR/PNG/iso_8859-1.txt"
#define XVM_URL "https://nightly.modxvm.com/download/master/xvm_latest.zip"

#if (FAST_URL == 1)
#undef XVM_URL
#define XVM_URL TEST_URL
#endif


int main()
{
	TempDownloader dl = TempDownloader(XVM_URL);
	if (dl.Download())
	{
		return 1;
	}
	
	
}
