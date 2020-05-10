#pragma once

#define TEST_URL "https://www.w3.org/TR/PNG/iso_8859-1.txt"
#define XVM_URL "https://nightly.modxvm.com/download/master/xvm_latest.zip"

#ifndef FAST_URL
  #define FAST_URL 0
#endif

#ifndef SKIP_DOWNLOAD
  #define SKIP_DOWNLOAD 0
#endif

#if (FAST_URL == 1)
  #undef XVM_URL
  #define XVM_URL TEST_URL
#endif
