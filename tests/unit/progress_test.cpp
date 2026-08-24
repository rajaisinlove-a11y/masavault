#include <cassert>
#include "tgcloud/transfer/progress.hpp"
int main(){tgcloud::transfer::Progress p(100);p.update(40);auto s=p.snapshot();assert(s.completed==40&&s.total==100&&s.bytes_per_second>=0&&s.eta_seconds>=0);}
