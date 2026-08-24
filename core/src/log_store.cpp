#include "tgcloud/sync/log_store.hpp"
namespace tgcloud::sync {void LogStore::append(model::SyncLog l){std::lock_guard g(m_);logs_.push_back(std::move(l));}std::vector<model::SyncLog> LogStore::pending()const{std::lock_guard g(m_);return logs_;}std::size_t LogStore::size()const{std::lock_guard g(m_);return logs_.size();}}
