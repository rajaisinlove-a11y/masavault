// Task 05 — in-memory Store: put / get / list cloud files with pagination.
// Read progress/README.md if you are new to this repo.
#include <cassert>
#include "tgcloud/database/memory_store.hpp"
int main(){tgcloud::database::MemoryStore s; tgcloud::model::CloudFile a; a.name="a"; auto id=s.put_file(a); assert(id==1); assert(s.get_file(id)->name=="a"); assert(s.list_files(1,0).size()==1); s.delete_file(id); assert(!s.get_file(id));}
