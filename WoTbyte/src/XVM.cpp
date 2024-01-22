#include <regex>

#include "XVM.h"
#include "WOT.h"
#include "Log.h"
#include "Http.h"
#include "Config.h"

#define DEFAULT_BRANCH      "master"

std::string XVM::MakeUrl(std::string branchName)
{
  Xvm xvm = Config::GetXvmData();
  return xvm.UrlStorage + branchName + "/" + xvm.Filename;
}

std::string XVM::SelectBranch()
{
  // Branch selection algorithm
  // 1. Check if branch is overriden - if so, use it, otherwise proceed to p.2
  // 2. Try to fetch branch from xvm website - otherwise proceed to p.3
  // 3. Fallback - get default branch

  std::string xvmBranchName = Config::GetXvmData().Branch;
  if (xvmBranchName.empty() == false)
  {
    LOG_DEBUG("Branch is overriden in config file to: {}", xvmBranchName);
    return xvmBranchName;
  }

  xvmBranchName = GetWebBranch();
  if (xvmBranchName.empty() == false)
  {
    LOG_DEBUG("Web branch: {}", xvmBranchName);
    return xvmBranchName;
  }

  LOG_DEBUG("Using default XVM branch: {}", DEFAULT_BRANCH);
  return DEFAULT_BRANCH;
}

std::string XVM::GetFileUrl()
{
  std::string branchName = SelectBranch();
  LOG_INFO("Selected XVM branch: {}", branchName);
  return MakeUrl(branchName);
}

std::string XVM::GetWebBranch()
{
  Http http;
  std::string data = http.Get(Config::GetXvmData().UrlBranches);
  auto branches = ExtractBranchInfo(data);
  std::string wotVersion = WOT::GetVersion();

  for (BranchInfo branch : branches)
  {
    if (branch.WotVersion == wotVersion)
    {
      LOG_DEBUG("Proper branch detected: {}, wot version: {}, url: {}", branch.Name, branch.WotVersion, branch.Url);
      return branch.Name;
    }
  }

  LOG_DEBUG("No related branch found!");
  return "";
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