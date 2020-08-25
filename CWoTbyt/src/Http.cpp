#include "Http.h"
#include "Log.h"

#define DLL_NAME "libcurl.dll"

Http::Http()
{
  HINSTANCE curlDll = LoadLibrary(DLL_NAME);
  if (curlDll == nullptr)
  {
    LOG_DEBUG("{} not found!", DLL_NAME);
    return;
  }
  
  m_curl_easy_init = (curl_easy_init_t)GetProcAddress(curlDll, "curl_easy_init");
  m_curl_easy_cleanup = (curl_easy_cleanup_t)GetProcAddress(curlDll, "curl_easy_cleanup");
  m_curl_easy_setopt = (curl_easy_setopt_t)GetProcAddress(curlDll, "curl_easy_setopt");
  m_curl_easy_perform = (curl_easy_perform_t)GetProcAddress(curlDll, "curl_easy_perform");
  m_curl_easy_strerror = (curl_easy_strerror_t)GetProcAddress(curlDll, "curl_easy_strerror");
  
  m_curl = m_curl_easy_init();
  if (m_curl == nullptr)
  {
    LOG_DEBUG("Curl initialization failed!");
    return;
  }

  m_isInitialized = true;
}

Http::~Http()
{
  if (m_curl)
  {
    m_curl_easy_cleanup(m_curl);
  }
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
  ((std::string*)userp)->append((char*)contents, size * nmemb);
  return size * nmemb;
}

std::string Http::Get(std::string url)
{
  std::string readBuffer;

  m_curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
  m_curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
  m_curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  m_curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &readBuffer);

  CURLcode res = m_curl_easy_perform(m_curl);
  if (res != CURLE_OK)
  {
    LOG_DEBUG("Getting page contents failed: {}, msg:", url, m_curl_easy_strerror(res));
  }
  return readBuffer;
}

const bool Http::IsInitialized()
{
  return m_isInitialized;
}