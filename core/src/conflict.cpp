#include "tgcloud/sync/conflict.hpp"
namespace tgcloud::sync {std::optional<Conflict> detect(const model::SyncLog&a,const model::SyncLog&b){if(a.table_name!=b.table_name||a.primary_key!=b.primary_key||a.data_json==b.data_json)return std::nullopt;return Conflict{a,b};}model::SyncLog merge(const Conflict&c){return c.local.timestamp>=c.remote.timestamp?c.local:c.remote;}}
