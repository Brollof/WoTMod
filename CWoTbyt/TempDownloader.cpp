#include "TempDownloader.h"
#include <urlmon.h>
#include <WinInet.h>
#include <filesystem>
#include <cstdio>

#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "wininet.lib")

TempDownloader::TempDownloader(const std::string& url) :
	m_DownloadOk(false), m_Url(url)
{
	m_Filename = std::filesystem::path(url).filename().string();
	m_OutPath = std::filesystem::temp_directory_path().concat(m_Filename).string();
}

TempDownloader::~TempDownloader()
{
	if (remove(m_OutPath.c_str()) == 0)
	{
		std::cout << "File deleted" << std::endl;
	}
	else
	{
		std::cout << "Failed to delete a file!" << std::endl;
	}
}

int TempDownloader::Download()
{
	std::cout << "Downloading file " << m_Url << "... ";

	DeleteUrlCacheEntry(m_Url.c_str());
	HRESULT result = URLDownloadToFile(NULL, m_Url.c_str(), m_OutPath.c_str(), 0, NULL);
	if (result != S_OK)
	{
		m_DownloadOk = false;
		std::cout << "failed!" << std::endl;
		return -1;
	}

	m_DownloadOk = true;
	std::cout << "OK\nSaved: " << m_OutPath << std::endl;
	return 0;
}
