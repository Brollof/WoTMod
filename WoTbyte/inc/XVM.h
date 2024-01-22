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
  static std::string GetFileUrl();
  static std::string SelectBranch();
  static std::string GetWebBranch();
  static std::string MakeUrl(std::string branchName);

private:
  XVM() {};
  static std::vector<BranchInfo> ExtractBranchInfo(std::string text);
};
