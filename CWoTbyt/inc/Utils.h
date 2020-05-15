#pragma once

#include <iostream>
#include <codecvt>
#include <AtlBase.h>
#include "conio.h"

#define STR_TO_WSTR(str) std::wstring(CA2W(str))
#define WSTR_TO_STR(str) std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(str)

static inline void AppEnd()
{
  std::cout << "Press any key to exit...";
  _getch();
}