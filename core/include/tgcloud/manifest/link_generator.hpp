#pragma once
#include <filesystem>
#include <vector>
#include "manifest.hpp"
namespace tgcloud::manifest { void generate(const FileManifest&,std::string_view,const std::filesystem::path&); void generate_batch(const std::vector<FileManifest>&,std::string_view,const std::filesystem::path&); }
