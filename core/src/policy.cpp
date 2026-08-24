#include "tgcloud/retry/policy.hpp"
namespace tgcloud::retry { Decision Policy::decide(uint32_t a,bool transient,uint32_t after)const{if(!transient||a>=max_)return{false,std::chrono::milliseconds(0)};auto d=after?after:base_*(1u<<a);return{true,std::chrono::milliseconds(d)};} }
