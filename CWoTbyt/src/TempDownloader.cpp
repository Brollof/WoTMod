#include <filesystem>
#include <urlmon.h>
#include <WinInet.h>
#include "TempDownloader.h"
#include "Log.h"

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
    LOG_DEBUG("Temporary ZIP file deleted");
  }
  else
  {
    LOG_DEBUG("Failed to delete the ZIP file!");
  }
}

bool TempDownloader::Download()
{
  LOG_INFO("Downloading file {}... ", m_url);

  DeleteUrlCacheEntry(m_url.c_str());

  HRESULT result = URLDownloadToFile(NULL, m_url.c_str(), m_outPath.c_str(), 0, NULL);
  if (result == S_OK) {
    LOG_DEBUG("Saved: {}", m_outPath);
    return true;
  }

  LOG_INFO("Download failed!");

  if (result == E_OUTOFMEMORY) {
    LOG_DEBUG("Buffer length invalid, or insufficient memory");
  }
  else if (result == INET_E_DOWNLOAD_FAILURE) {
    LOG_DEBUG("URL is invalid");
  }
  else {
    LOG_DEBUG("Other error: {}", result);
  }

  return false;
}