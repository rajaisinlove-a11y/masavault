#include <cassert>
#include "tgcloud/sync/chain.hpp"
int main(){using namespace tgcloud::sync;std::vector<Node>n={{"a","",1,{}},{"b","a",2,{}}};auto x=traverse(n,"b");assert(x.size()==2&&x[0].id=="a");}
