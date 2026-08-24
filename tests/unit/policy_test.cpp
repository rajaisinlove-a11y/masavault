#include <cassert>
#include "tgcloud/retry/policy.hpp"
int main(){tgcloud::retry::Policy p(3,10);auto a=p.decide(0,true),b=p.decide(1,true,99),c=p.decide(3,true);assert(a.retry&&a.delay.count()==10);assert(b.delay.count()==99);assert(!c.retry);}
