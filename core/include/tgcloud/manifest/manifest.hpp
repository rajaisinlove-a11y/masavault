#pragma once
#include <string>
#include "tgcloud/model/domain.hpp"
namespace tgcloud::manifest { struct FileManifest { std::string version{"1"}; model::CloudFile file; }; std::string to_json(const FileManifest&); }
