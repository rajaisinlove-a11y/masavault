#pragma once
#include <string>
#include <vector>
#include "tgcloud/model/domain.hpp"
namespace tgcloud::sync { struct Node{std::string id,prev_id;std::int64_t timestamp{};std::vector<model::SyncLog> entries;};std::string node_json(const Node&);std::string encrypt_node(const Node&,std::string_view password); }
