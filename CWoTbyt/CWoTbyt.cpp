// CWoTbyt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "TempDownloader.h"

#define XVM_URL "https://nightly.modxvm.com/download/master/xvm_latest.zip"


int main()
{
	TempDownloader dl = TempDownloader(XVM_URL);
	if (dl.Download())
	{
		return 1;
	}
	
	
}
