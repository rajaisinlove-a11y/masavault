#pragma once
#include <optional>
#include <vector>
#include "tgcloud/model/domain.hpp"
namespace tgcloud::database {
class Store {
public:
 virtual ~Store() = default;
 virtual model::Id put_file(model::CloudFile file) = 0;
 virtual std::optional<model::CloudFile> get_file(model::Id id) const = 0;
 virtual std::vector<model::CloudFile> list_files(std::size_t limit, std::size_t offset) const = 0;
 virtual void delete_file(model::Id id) = 0;
};
}
