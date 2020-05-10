#pragma once

#include "bitextractor.hpp"

class ZipExtractor
{
public:
  ZipExtractor();
  virtual ~ZipExtractor();
  bool Extract(const std::string& zipPath, const std::string& outDir);

private:
  const bit7z::Bit7zLibrary* m_lib = nullptr;
  const bit7z::BitExtractor* m_extractor = nullptr;
};

