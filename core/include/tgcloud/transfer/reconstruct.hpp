#pragma once
#include <filesystem>
#include <vector>
#include "tgcloud/model/domain.hpp"
namespace tgcloud::transfer { void reconstruct(const std::vector<model::ChunkRecord>& chunks,const std::filesystem::path& parts,const std::filesystem::path& output); }
