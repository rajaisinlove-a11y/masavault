// Task 14 — bounded worker queue runs jobs and shuts down cleanly.
// Read progress/README.md if you are new to this repo.
#include <atomic>
#include <cassert>
#include <thread>
#include "tgcloud/transfer/queue.hpp"
int main(){std::atomic<int> n=0;{tgcloud::transfer::TransferQueue q(3);for(int i=0;i<20;i++)q.submit([&]{++n;});}assert(n==20);}
