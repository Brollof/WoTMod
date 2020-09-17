#include <iostream>
#include <sstream>
#include <filesystem>
#include <urlmon.h>
#include <WinInet.h>
#include "TempDownloader.h"
#include "Log.h"

#define DOT_MAX_N 70

#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "wininet.lib")

class DownloadStatusCb : public IBindStatusCallback
{
public:
  DownloadStatusCb() {}
  ~DownloadStatusCb() {}

  STDMETHOD(OnProgress)(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR wszStatusText)
  {
    // We don't care about current status (ulStatusCode) - progress bar will be always shown

    std::ostringstream progressBar;
    int percent = 0;
    int dots = 0;

    if (ulProgressMax)
    {
      percent = (ulProgress * 100) / ulProgressMax;
      dots = (ulProgress * DOT_MAX_N) / ulProgressMax;
    }

    progressBar << "\r" << std::setw(3) << percent << "% [" \
      << std::string(dots, '.') << std::string(DOT_MAX_N - dots, ' ') \
      << "] " << ulProgress << "/" << ulProgressMax << " bytes";

    std::cout << progressBar.str();

    return S_OK;
  }

  // Unused methods
  STDMETHOD(OnStartBinding)(DWORD dwReserved, IBinding __RPC_FAR* pib) { return E_NOTIMPL; }
  STDMETHOD(GetPriority)(LONG __RPC_FAR* pnPriority) { return E_NOTIMPL; }
  STDMETHOD(OnLowResource)(DWORD reserved) { return E_NOTIMPL; }
  STDMETHOD(OnStopBinding)(HRESULT hresult, LPCWSTR szError) { return E_NOTIMPL; }
  STDMETHOD(GetBindInfo)(DWORD __RPC_FAR* grfBINDF, BINDINFO __RPC_FAR* pbindinfo) { return E_NOTIMPL; }
  STDMETHOD(OnDataAvailable)(DWORD grfBSCF, DWORD dwSize, FORMATETC __RPC_FAR* pformatetc, STGMEDIUM __RPC_FAR* pstgmed) { return E_NOTIMPL; }
  STDMETHOD(OnObjectAvailable)(REFIID riid, IUnknown __RPC_FAR* punk) { return E_NOTIMPL; }
  STDMETHOD_(ULONG, AddRef)() { return 0; }
  STDMETHOD_(ULONG, Release)() { return 0; }
  STDMETHOD(QueryInterface)(REFIID riid, void __RPC_FAR* __RPC_FAR* ppvObject) { return E_NOTIMPL; }
};

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

  DownloadStatusCb cb;
  uint32_t result = URLDownloadToFile(NULL, m_url.c_str(), m_outPath.c_str(), 0, &cb);
  std::cout << std::endl; // add new line because progress bar uses carriage return only
  if (result == S_OK)
  {
    LOG_DEBUG("Saved: {}", m_outPath);
    return true;
  }

  LOG_INFO("Download failed!");

  if (result == E_OUTOFMEMORY)
  {
    LOG_DEBUG("Buffer length invalid, or insufficient memory");
  }
  else if (result == INET_E_DOWNLOAD_FAILURE)
  {
    LOG_DEBUG("URL is invalid");
  }
  else
  {
    LOG_DEBUG("Other error: 0x{0:X}", result);
    LOG_DEBUG("Check for more information: https://docs.microsoft.com/en-us/previous-versions/windows/internet-explorer/ie-developer/platform-apis/ms775145(v=vs.85)");
  }

  return false;
}