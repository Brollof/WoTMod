#include "XVM.h"
#include "WOT.h"
#include "Log.h"
#include <regex>

#define XVM_HOME          "https://nightly.modxvm.com/"
#define XVM_STORAGE       "https://nightly.modxvm.com/download/"
#define XVM_FILENAME      "xvm_latest.zip"

std::string XVM::GetUrl(std::string branchName)
{
  return XVM_STORAGE + branchName + "/" + XVM_FILENAME;
}

std::string XVM::GetUrl(std::string wotPath, std::string webContent)
{
  auto branches = ExtractBranchInfo(webContent);
  std::string wotVersion = WOT::ExtractWotVersion(wotPath);

  for (auto branch : branches)
  {
    if (branch.WotVersion == wotVersion)
    {
      LOG_DEBUG("Proper branch detected: {}, wot version: {}, url: {}", branch.Name, branch.WotVersion, branch.Url);
      return GetUrl(branch.Name);
    }
  }
  LOG_DEBUG("No related branch found!");
  return "";
}

std::string XVM::GetXvmWebsite()
{
  return XVM_HOME;
}

std::vector<BranchInfo> XVM::ExtractBranchInfo(std::string text)
{
  LOG_DEBUG("Parsing web content...");
  std::smatch res;
  std::regex exp("<td>(\\d+\\.\\d+\\.\\d+\\.\\d+)</td><td><a href=\"(.*?/(.*?)/.*?)\"");
  std::vector<BranchInfo> v;

  while (std::regex_search(text, res, exp))
  {
    v.push_back({ res[3], res[1], res[2] });
    text = res.suffix();
  }

  LOG_DEBUG("Found {} branches", v.size());
  return v;
}