#include <cassert>
#include "tgcloud/compat/cloud_files.hpp"
#include "tgcloud/database/memory_store.hpp"
int main(){tgcloud::database::MemoryStore s;tgcloud::compat::CloudFiles c(s);tgcloud::model::CloudFile f;f.file_uuid="u";f.telegram_file_id="remote";f.name="x";auto id=c.upsert(f);assert(c.by_telegram_file("remote")->local_id==id);f.name="y";c.upsert(f);assert(s.list_files(9,0).size()==2);}
