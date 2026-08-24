#pragma once
#include <filesystem>
#include <string>
#include "memory_store.hpp"
namespace tgcloud::database {
class EncryptedFileStore final : public Store {
 MemoryStore memory_; std::filesystem::path path_; std::string password_;
public:
 EncryptedFileStore(std::filesystem::path path, std::string password);
 void load(); void flush() const;
 model::Id put_file(model::CloudFile file) override { auto id=memory_.put_file(std::move(file)); flush(); return id; }
 std::optional<model::CloudFile> get_file(model::Id id) const override { return memory_.get_file(id); }
 std::vector<model::CloudFile> list_files(std::size_t l,std::size_t o) const override { return memory_.list_files(l,o); }
 void delete_file(model::Id id) override { memory_.delete_file(id); flush(); }
}; }
