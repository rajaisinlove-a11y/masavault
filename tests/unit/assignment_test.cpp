// Task 13 — each chunk gets an uploader alias from the BotPool.
// Read progress/README.md if you are new to this repo.
#include <cassert>
#include "tgcloud/transfer/assignment.hpp"
int main(){using namespace tgcloud; scheduler::BotPool p({{"a"},{"b"}});std::vector<model::ChunkRecord> c(3);auto a=transfer::assign_chunks(c,p);assert(a.size()==2&&c[0].uploader_alias=="a"&&c[1].uploader_alias=="b");}
