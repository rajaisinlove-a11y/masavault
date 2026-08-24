// Task 31 — export/import CloudFile fields without dropping Telegram ids.
// Read progress/README.md if you are new to this repo.
#include <cassert>
#include "tgcloud/migration/migrator.hpp"
int main(){tgcloud::model::CloudFile f;f.local_id=3;f.name="x";f.telegram_file_id="fid";auto r=tgcloud::migration::import_file(tgcloud::migration::export_file(f));assert(r.local_id==3&&r.name=="x"&&r.telegram_file_id=="fid");}
