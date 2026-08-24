#include "tgcloud/files/folders.hpp"
#include <atomic>
namespace tgcloud::files {Folder FolderTree::create(std::string n,std::string p){static std::atomic<unsigned> next{1};Folder f{"folder-"+std::to_string(next++),std::move(n),std::move(p)};folders_.push_back(f);return f;}std::vector<std::string> select_all_for_share(const std::vector<std::string>&ids){return ids;}}
