#pragma once
#include <string>
#include <vector>
namespace tgcloud::desktop { struct ShareAction{std::vector<std::string> ids;bool batch{false};}; struct BackupAction{std::string path;bool export_mode{true};}; struct GalleryFilter{std::string mime;bool images_only{false};}; class ActionsModel{public:ShareAction share(std::vector<std::string> ids)const{return{std::move(ids),true};}BackupAction export_backup(std::string p)const{return{std::move(p),true};}}; }
