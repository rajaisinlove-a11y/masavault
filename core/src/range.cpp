#include "tgcloud/streaming/range.hpp"
#include <algorithm>
namespace tgcloud::streaming {std::vector<Range> plan(uint64_t start,uint64_t len,uint64_t size,uint32_t total){std::vector<Range>r;if(!len||!size)return r;auto end=std::min(start+len,size*total);for(auto i=(uint32_t)(start/size);i<total&&i*size<end;i++){auto b=std::max(start,(uint64_t)i*size),e=std::min(end,(uint64_t)(i+1)*size);r.push_back({i,b-(uint64_t)i*size,e-b});}return r;}}
