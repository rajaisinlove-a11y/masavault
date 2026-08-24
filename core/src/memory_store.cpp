#include "tgcloud/database/memory_store.hpp"
namespace tgcloud::database {
model::Id MemoryStore::put_file(model::CloudFile file) { if (!file.local_id) file.local_id=next_id_++; else if (file.local_id>=next_id_) next_id_=file.local_id+1; files_[file.local_id]=std::move(file); return files_.at(file.local_id).local_id; }
std::optional<model::CloudFile> MemoryStore::get_file(model::Id id) const { auto it=files_.find(id); return it==files_.end()?std::nullopt:std::optional<model::CloudFile>(it->second); }
std::vector<model::CloudFile> MemoryStore::list_files(std::size_t limit,std::size_t offset) const { std::vector<model::CloudFile> out; for(auto it=files_.begin();it!=files_.end()&&out.size()<limit;++it) if(offset) --offset; else out.push_back(it->second); return out; }
void MemoryStore::delete_file(model::Id id) { files_.erase(id); }
}
