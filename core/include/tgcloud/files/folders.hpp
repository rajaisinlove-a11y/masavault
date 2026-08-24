#pragma once
#include <string>
#include <vector>
namespace tgcloud::files { struct Folder{std::string id,name,parent_id;}; class FolderTree{std::vector<Folder> folders_; public: Folder create(std::string name,std::string parent=""); const std::vector<Folder>& all()const{return folders_;} }; std::vector<std::string> select_all_for_share(const std::vector<std::string>& ids); }
