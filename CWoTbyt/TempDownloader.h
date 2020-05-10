#pragma once
#include <iostream>

class TempDownloader
{
private:
	std::string m_Url;
	std::string m_OutPath;
	std::string m_Filename;
	bool m_DownloadOk;

public:
	TempDownloader(const std::string& url);
	virtual ~TempDownloader();
	int Download();
	const std::string& GetFilename() const { return m_Filename; }
	const std::string& GetOutPath() const { return m_OutPath; }
};

