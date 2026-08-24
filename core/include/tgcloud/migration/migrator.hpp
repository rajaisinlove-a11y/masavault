#pragma once
#include <string>
#include "tgcloud/model/domain.hpp"
namespace tgcloud::migration { std::string export_file(const model::CloudFile&); model::CloudFile import_file(std::string_view); }
