#pragma once
#include <optional>
#include "tgcloud/model/domain.hpp"
namespace tgcloud::sync { struct Conflict{model::SyncLog local,remote;}; std::optional<Conflict> detect(const model::SyncLog&,const model::SyncLog&); model::SyncLog merge(const Conflict&); }
