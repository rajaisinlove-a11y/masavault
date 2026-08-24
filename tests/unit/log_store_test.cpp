#include <cassert>
#include "tgcloud/sync/log_store.hpp"
int main(){tgcloud::sync::LogStore s;tgcloud::model::SyncLog l;l.log_id="1";s.append(l);assert(s.size()==1&&s.pending()[0].log_id=="1");}
