#include <cassert>
#include "tgcloud/files/browser.hpp"
int main(){tgcloud::files::BrowserBackend b;assert(b.uri("root")=="tgcloud://root");assert(b.children("root").empty());}
