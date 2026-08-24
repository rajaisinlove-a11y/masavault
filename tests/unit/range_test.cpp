#include <cassert>
#include "tgcloud/streaming/range.hpp"
int main(){auto r=tgcloud::streaming::plan(5,12,10,3);assert(r.size()==2&&r[0].index==0&&r[0].length==5&&r[1].index==1&&r[1].length==7);}
