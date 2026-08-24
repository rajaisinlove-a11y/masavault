#pragma once
#include <optional>
#include <string>
#include "tgcloud/database/store.hpp"
namespace tgcloud::compat { class CloudFiles { database::Store& store_; public: explicit CloudFiles(database::Store&s):store_(s){} model::Id upsert(model::CloudFile f); std::optional<model::CloudFile> by_telegram_file(std::string id)const; }; }
