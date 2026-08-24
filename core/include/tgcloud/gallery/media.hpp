#pragma once
#include <string>
#include <cstdint>
#include <cstddef>
#include <vector>
namespace tgcloud::gallery { struct Media{std::string id,path,name,mime,remote_file_id;std::uint64_t size{};std::int64_t date_taken{},date_modified{};std::uint32_t width{},height{};std::uint64_t duration_ms{};bool synced{false};}; class Catalog{std::vector<Media> items_;public:void add(Media m);std::vector<Media> list(std::size_t limit,std::size_t offset)const;}; }
