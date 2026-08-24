// Task 36 — Sidebar / dashboard navigation model
//
// Desktop pages must match a normal cloud-drive sidebar.
// This test lists those pages and checks that navigate() sticks.

#include <cassert>
#include "desktop/ui/navigation.hpp"

int main() {
    using tgcloud::desktop::NavigationModel;
    using tgcloud::desktop::Page;

    auto items = NavigationModel::items();
    assert(items.size() == 9);
    assert(items.front().page == Page::home);
    assert(items.front().label == "Home");
    assert(items.back().page == Page::settings);
    assert(items.back().label == "Settings");

    NavigationModel nav;
    assert(nav.current() == Page::home);
    nav.navigate(Page::transfers);
    assert(nav.current() == Page::transfers);
    nav.navigate(Page::gallery);
    assert(nav.current() == Page::gallery);
    return 0;
}
