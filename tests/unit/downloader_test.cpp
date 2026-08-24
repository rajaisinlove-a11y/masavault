// Task 15 — Downloader is a thin BotClient wrapper (no network in this unit test).
// Read progress/README.md if you are new to this repo.
#include <cassert>
#include "tgcloud/transfer/downloader.hpp"
int main(){tgcloud::telegram::BotClient c("TEST");tgcloud::transfer::Downloader d(c);assert(d.fetch("http://127.0.0.1:1/no","/tmp/no-such-file").http_status==0);}
