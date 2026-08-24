#pragma once
#include <string>
#include <string_view>
#include "tgcloud/model/domain.hpp"
namespace tgcloud::manifest {
struct FileManifest {
    std::string version{"1"};
    model::CloudFile file;
};
std::string to_json(const FileManifest&);
FileManifest from_json(std::string_view json);
}
