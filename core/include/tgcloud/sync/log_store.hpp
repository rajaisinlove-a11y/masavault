#pragma once
#include <vector>
#include <mutex>
#include "tgcloud/model/domain.hpp"
namespace tgcloud::sync { class LogStore { std::vector<model::SyncLog> logs_; mutable std::mutex m_; public: void append(model::SyncLog log); std::vector<model::SyncLog> pending() const; std::size_t size() const; }; }
