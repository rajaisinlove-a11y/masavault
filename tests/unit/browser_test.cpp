// Task 34 — tgcloud:// URI helper for a future file-manager backend.
// Read progress/README.md if you are new to this repo.
#include <cassert>
#include "tgcloud/files/browser.hpp"
int main(){tgcloud::files::BrowserBackend b;assert(b.uri("root")=="tgcloud://root");assert(b.children("root").empty());}
