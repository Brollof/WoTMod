#pragma once

#include <codecvt>
#include <AtlBase.h>

#define STR_TO_WSTR(str) std::wstring(CA2W(str))
#define WSTR_TO_STR(str) std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(str)