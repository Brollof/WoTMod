#pragma once

#include <string>
#include <vector>

struct BranchInfo
{
  std::string Name;
  std::string WotVersion;
  std::string Url;
};

class XVM
{
public:
  static std::string GetUrl(std::string branchName);
  static std::string GetUrl(std::string wotPath, std::string webContent);
  static std::string GetXvmWebsite();

private:
  XVM() {};
  static std::vector<BranchInfo> ExtractBranchInfo(std::string text);

};