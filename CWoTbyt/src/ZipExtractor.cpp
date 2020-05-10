#include <iostream>
#include "ZipExtractor.h"
#include "bitexception.hpp"

#define DLL_7Z_NAME L"7z.dll"

ZipExtractor::ZipExtractor()
{
  try
  {
    m_lib = new bit7z::Bit7zLibrary(DLL_7Z_NAME);
    m_extractor = new bit7z::BitExtractor(*m_lib, bit7z::BitFormat::Zip);
    std::cout << "ZIP extractor initialized" << std::endl;
  }
  catch (const bit7z::BitException& ex)
  {
    std::cout << "Exception: " << ex.what() << std::endl;
  }
}

ZipExtractor::~ZipExtractor()
{
  delete m_extractor;
}

bool ZipExtractor::Extract(const std::string &zipPath, const std::string& outDir)
{
  std::cout << "Extracting from file: '" << zipPath << "' to: '" << outDir << "'" << std::endl;
  std::wstring wZipPath(zipPath.begin(), zipPath.end());
  std::wstring wOutDir(outDir.begin(), outDir.end());

  try
  {
    m_extractor->extract(wZipPath, wOutDir);
  }
  catch (const bit7z::BitException& ex)
  {
    std::cout << "Exception: " << ex.what() << std::endl;
    return false;
  }

  std::cout << "Extraction successful" << std::endl;
  return true;
}