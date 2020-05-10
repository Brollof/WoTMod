#include "TempDownloader.h"
#include <urlmon.h>
#include <WinInet.h>
#include <filesystem>
#include <iostream>

#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "wininet.lib")

TempDownloader::TempDownloader(const std::string& url)
{
  m_url = url;
  m_filename = std::filesystem::path(url).filename().string();
  m_outPath = std::filesystem::temp_directory_path().concat(m_filename).string();
}

TempDownloader::~TempDownloader()
{
  if (remove(m_outPath.c_str()) == 0)
  {
    std::cout << "File deleted" << std::endl;
  }
  else
  {
    std::cout << "Failed to delete the file!" << std::endl;
  }
}

bool TempDownloader::Download()
{
  std::cout << "Downloading file " << m_url << "... ";

  DeleteUrlCacheEntry(m_url.c_str());
  HRESULT result = URLDownloadToFile(NULL, m_url.c_str(), m_outPath.c_str(), 0, NULL);

  if (result == S_OK) {
    std::cout << "OK\nSaved: " << m_outPath << std::endl;
    return true;
  }
  else if (result == E_OUTOFMEMORY) {
    std::cout << "Buffer length invalid, or insufficient memory" << std::endl;
  }
  else if (result == INET_E_DOWNLOAD_FAILURE) {
    std::cout << "URL is invalid" << std::endl;
  }
  else {
    std::cout << "Other error: " << result << std::endl;
  }

  return false;
}