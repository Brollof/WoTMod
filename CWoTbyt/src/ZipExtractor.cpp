#include "ZipExtractor.h"
#include "bitexception.hpp"
#include "Log.h"

#define DLL_7Z_NAME L"7z.dll"

ZipExtractor::ZipExtractor()
{
  try
  {
    m_lib = new bit7z::Bit7zLibrary(DLL_7Z_NAME);
    m_extractor = new bit7z::BitExtractor(*m_lib, bit7z::BitFormat::Zip);
    LOG_DEBUG("ZIP extractor initialized");
  }
  catch (const bit7z::BitException& ex)
  {
    LOG_ERROR("Extraction failed!");
    LOG_DEBUG("Exception: {}", ex.what());
  }
}

ZipExtractor::~ZipExtractor()
{
  delete m_extractor;
}

bool ZipExtractor::Extract(const std::string &zipPath, const std::string& outDir)
{
  LOG_INFO("Extracting archive: '{}' to '{}'", zipPath, outDir);
  std::wstring wZipPath(zipPath.begin(), zipPath.end());
  std::wstring wOutDir(outDir.begin(), outDir.end());

  try
  {
    m_extractor->extract(wZipPath, wOutDir);
  }
  catch (const bit7z::BitException& ex)
  {
    LOG_ERROR("Extraction failed!");
    LOG_DEBUG("Exception: {}", ex.what());
    return false;
  }

  LOG_DEBUG("Extraction successful");
  return true;
}