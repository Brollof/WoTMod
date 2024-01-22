#pragma once

#include <string>
#include "curl/curl.h"

typedef CURL* (*curl_easy_init_t)();
typedef void (*curl_easy_cleanup_t)(CURL* curl);
typedef CURLcode(*curl_easy_setopt_t)(CURL* curl, CURLoption option, ...);
typedef CURLcode(*curl_easy_perform_t)(CURL* curl);
typedef const char* (*curl_easy_strerror_t)(CURLcode);

class Http
{
public:
  Http();
  ~Http();
  std::string Get(std::string url);
  const bool IsInitialized();

private:
  bool m_isInitialized = false;
  CURL* m_curl = nullptr;
  curl_easy_init_t m_curl_easy_init = nullptr;
  curl_easy_cleanup_t m_curl_easy_cleanup = nullptr;
  curl_easy_setopt_t m_curl_easy_setopt = nullptr;
  curl_easy_perform_t m_curl_easy_perform = nullptr;
  curl_easy_strerror_t m_curl_easy_strerror = nullptr;
};