#pragma once
#include <map>
#include "store.hpp"
namespace tgcloud::database {
class MemoryStore final : public Store {
 std::map<model::Id, model::CloudFile> files_;
 model::Id next_id_{1};
public:
 model::Id put_file(model::CloudFile file) override;
 std::optional<model::CloudFile> get_file(model::Id id) const override;
 std::vector<model::CloudFile> list_files(std::size_t limit, std::size_t offset) const override;
 void delete_file(model::Id id) override;
};
}
