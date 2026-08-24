// Task 25 — same-record different payload → later timestamp wins.
// Read progress/README.md if you are new to this repo.
#include <cassert>
#include "tgcloud/sync/conflict.hpp"
int main(){tgcloud::model::SyncLog a,b;a.table_name=b.table_name="cloud_files";a.primary_key=b.primary_key="1";a.data_json="a";b.data_json="b";a.timestamp=1;b.timestamp=2;auto c=tgcloud::sync::detect(a,b);assert(c);assert(tgcloud::sync::merge(*c).data_json=="b");}
