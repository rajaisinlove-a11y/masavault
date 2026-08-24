#include "tgcloud/transfer/progress.hpp"
namespace tgcloud::transfer {void Progress::update(uint64_t n){std::lock_guard l(m_);done_=n>total_?total_:n;} ProgressSnapshot Progress::snapshot()const{std::lock_guard l(m_);auto sec=std::chrono::duration<double>(std::chrono::steady_clock::now()-start_).count();double rate=sec>0?done_/sec:0;return{done_,total_,rate,rate>0?(total_-done_)/rate:0};}}
