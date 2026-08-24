#pragma once
#include <cstdint>
#include <vector>
namespace tgcloud::streaming { struct Range{std::uint32_t index;std::uint64_t offset,length;}; std::vector<Range> plan(std::uint64_t start,std::uint64_t length,std::uint64_t chunk_size,std::uint32_t total); }
