#include <cassert>
#include "tgcloud/files/folders.hpp"
int main(){tgcloud::files::FolderTree t;auto f=t.create("Photos");auto c=t.create("2026",f.id);assert(f.name=="Photos"&&c.parent_id==f.id);std::vector<std::string>x={"a","b"};assert(tgcloud::files::select_all_for_share(x).size()==2);}
