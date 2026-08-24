// Task 35 — Transfer center UI model
//
// The future window will list uploads/downloads. This test checks the
// model that window will bind to: one row holds the task, a progress
// snapshot, and which bot aliases are currently working.

#include <cassert>
#include "desktop/ui/transfer_center.hpp"

int main() {
    using tgcloud::desktop::TransferCenterModel;
    using tgcloud::desktop::TransferRow;
    using tgcloud::model::TransferTask;
    using tgcloud::model::TransferState;

    TransferRow row;
    row.task.display_name = "movie.mp4";
    row.task.size_bytes = 100;
    row.task.state = TransferState::active;
    row.progress.completed = 40;
    row.progress.total = 100;
    row.active_bots = {"b1", "b2"};

    TransferCenterModel center;
    center.add(row);

    assert(center.rows().size() == 1);
    assert(center.rows()[0].task.display_name == "movie.mp4");
    assert(center.rows()[0].progress.completed == 40);
    assert(center.rows()[0].active_bots.size() == 2);
    return 0;
}
