#pragma once
#include <cstdint>
#include <filesystem>
#include <functional>
#include <string>
namespace tgcloud::chunking { constexpr std::uint64_t compatible_chunk_size=4ULL*1024*1024; struct Chunk {std::uint32_t index{},total{};std::uint64_t size{};std::string sha256;}; using ChunkConsumer=std::function<void(const Chunk&,const std::filesystem::path&)>; std::uint32_t count(std::uint64_t bytes); void split(const std::filesystem::path&,const std::filesystem::path&,ChunkConsumer); }
