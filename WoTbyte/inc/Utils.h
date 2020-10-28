#pragma once

#include <iostream>
#include <codecvt>
#include <AtlBase.h>
#include "conio.h"
#include <filesystem>

#define STR_TO_WSTR(str) std::wstring(CA2W(str))
#define WSTR_TO_STR(str) std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(str)

static inline void AppEnd()
{
  std::cout << "Press any key to exit...";
  _getch();
}

static inline std::string GetCurrentDir()
{
  char buffer[MAX_PATH];
  GetModuleFileNameA(NULL, buffer, MAX_PATH);
  auto pos = std::string(buffer).find_last_of("\\/");
  return std::string(buffer).substr(0, pos);
}

static inline const std::string FullPath(const std::string& filename)
{
  return (std::filesystem::path(GetCurrentDir()) / filename).string();
}