// Task 37 — Setup wizard model
//
// Storage channel, sync channel, device id, and bot *aliases* live here.
// Raw bot tokens and passwords must never enter this model.

#include <cassert>
#include "desktop/ui/setup.hpp"

int main() {
    tgcloud::desktop::SetupWizard wizard;
    wizard.set_storage_channel("-100000");
    wizard.set_sync_channel("-100001");
    wizard.add_bot_alias("storage-bot-1");
    wizard.add_bot_alias("storage-bot-2");

    const auto& cfg = wizard.config();
    assert(cfg.storage_channel == "-100000");
    assert(cfg.sync_channel == "-100001");
    assert(cfg.bot_aliases.size() == 2);
    assert(cfg.bot_aliases[0] == "storage-bot-1");

    // Guard: the config struct has no token / password fields.
    // If someone adds one, this comment is the review flag — keep aliases only.
    return 0;
}
