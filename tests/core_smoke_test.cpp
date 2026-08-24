// Task 03 — smoke: the core library links and the product name is TG Cloud Desktop.
// Read progress/README.md if you are new to this repo.
#include <cassert>
#include "tgcloud/core.hpp"
int main() {
    assert(tgcloud::product_name() == "TG Cloud Desktop");
    return 0;
}
