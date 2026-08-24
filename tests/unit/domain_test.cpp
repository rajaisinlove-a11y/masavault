// Task 04 — domain models: a CloudFile owns indexed chunks; transfers start queued; sync logs default to insert.
// Read progress/README.md if you are new to this repo.
#include <cassert>
#include "tgcloud/model/domain.hpp"
int main() { using namespace tgcloud::model; CloudFile f; f.chunks.push_back({2,3,4,"hash",9,"file","unique","bot-a"}); assert(f.chunks[0].index == 2); TransferTask t; assert(t.state == TransferState::queued); SyncLog s; assert(s.operation == SyncOperation::insert); }
