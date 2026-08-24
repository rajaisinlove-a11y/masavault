// Task 38 — Share / backup / gallery action model
//
// Select-all sharing must keep every file id (needed for a .link manifest).
// Backup export is a path + export flag. Gallery filter is MIME / images-only.

#include <cassert>
#include "desktop/ui/actions.hpp"

int main() {
    tgcloud::desktop::ActionsModel actions;

    auto share = actions.share({"a", "b", "c"});
    assert(share.batch);
    assert(share.ids.size() == 3);
    assert(share.ids[0] == "a");
    assert(share.ids[2] == "c");

    auto backup = actions.export_backup("/tmp/vault.bkp");
    assert(backup.export_mode);
    assert(backup.path == "/tmp/vault.bkp");

    tgcloud::desktop::GalleryFilter images;
    images.mime = "image/";
    images.images_only = true;
    assert(images.images_only);
    return 0;
}
