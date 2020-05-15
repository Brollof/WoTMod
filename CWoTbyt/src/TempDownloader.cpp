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

  STDMETHOD(OnProgress)(/* [in] */ ULONG ulProgress, /* [in] */ ULONG ulProgressMax, /* [in] */ ULONG ulStatusCode, /* [in] */ LPCWSTR wszStatusText)
  {
    std::stringstream progressBar;
    int percent = 0;
    int dots = 0;

    if (ulProgressMax)
    {
      percent = (ulProgress * 100) / ulProgressMax;
      dots = (DOT_MAX_N * ulProgress) / ulProgressMax;
    }

    progressBar << "\r" << std::setw(3) << percent << "% [";

    for (int i = 0; i < DOT_MAX_N; i++)
    {
      if (dots)
      {
        progressBar << ".";
        dots--;
      }
      else
      {
        progressBar << " ";
      }
    }

    progressBar << "] " << ulProgress << "/" << ulProgressMax;
    std::cout << progressBar.str();

    return S_OK;
  }

  STDMETHOD(OnStartBinding)(/* [in] */ DWORD dwReserved, /* [in] */ IBinding __RPC_FAR* pib)
  {
    return E_NOTIMPL;
  }

  STDMETHOD(GetPriority)(/* [out] */ LONG __RPC_FAR* pnPriority)
  {
    return E_NOTIMPL;
  }

  STDMETHOD(OnLowResource)(/* [in] */ DWORD reserved)
  {
    return E_NOTIMPL;
  }

  STDMETHOD(OnStopBinding)(/* [in] */ HRESULT hresult, /* [unique][in] */ LPCWSTR szError)
  {
    return E_NOTIMPL;
  }

  STDMETHOD(GetBindInfo)(/* [out] */ DWORD __RPC_FAR* grfBINDF, /* [unique][out][in] */ BINDINFO __RPC_FAR* pbindinfo)
  {
    return E_NOTIMPL;
  }

  STDMETHOD(OnDataAvailable)(/* [in] */ DWORD grfBSCF, /* [in] */ DWORD dwSize, /* [in] */ FORMATETC __RPC_FAR* pformatetc, /* [in] */ STGMEDIUM __RPC_FAR* pstgmed)
  {
    return E_NOTIMPL;
  }

  STDMETHOD(OnObjectAvailable)(/* [in] */ REFIID riid, /* [iid_is][in] */ IUnknown __RPC_FAR* punk)
  {
    return E_NOTIMPL;
  }

  STDMETHOD_(ULONG, AddRef)()
  {
    return 0;
  }

  STDMETHOD_(ULONG, Release)()
  {
    return 0;
  }

  STDMETHOD(QueryInterface)(/* [in] */ REFIID riid, /* [iid_is][out] */ void __RPC_FAR* __RPC_FAR* ppvObject)
  {
    return E_NOTIMPL;
  }
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
  HRESULT result = URLDownloadToFile(NULL, m_url.c_str(), m_outPath.c_str(), 0, &cb);
  std::cout << std::endl; // add new line because progress bar uses carriage return only
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