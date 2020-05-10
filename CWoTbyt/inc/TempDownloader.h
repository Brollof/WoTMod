#pragma once

#include <string>

class TempDownloader
{
private:
  std::string m_url;
  std::string m_outPath;
  std::string m_filename;

public:
  TempDownloader(const std::string& url);
  virtual ~TempDownloader();
  bool Download();
  const std::string& GetFilename() const { return m_filename; }
  const std::string& GetOutPath() const { return m_outPath; }
};

