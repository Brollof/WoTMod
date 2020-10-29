#pragma once

#include "bitextractor.hpp"

class Extractor
{
public:
  Extractor();
  virtual ~Extractor();
  bool Extract(const std::string& zipPath, const std::string& outDir);

private:
  const bit7z::Bit7zLibrary* m_lib = nullptr;
  const bit7z::BitExtractor* m_extractor = nullptr;
};

