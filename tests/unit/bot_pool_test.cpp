// Task 07 — BotPool: enabled bots are acquired round-robin; disabled bots are skipped.
// Read progress/README.md if you are new to this repo.
#include <cassert>
#include "tgcloud/scheduler/bot_pool.hpp"
int main(){using namespace tgcloud::scheduler;BotPool p({{"a"},{"b"},{"c"}});auto x=p.acquire(),y=p.acquire(),z=p.acquire();assert(x==0&&y==1&&z==2);p.release(y,false);auto s=p.snapshot();assert(s[1].failures==1&&s[1].active==0);}
