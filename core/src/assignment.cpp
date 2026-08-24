#include "tgcloud/transfer/assignment.hpp"
namespace tgcloud::transfer { std::vector<int> assign_chunks(std::vector<model::ChunkRecord>& cs,scheduler::BotPool&p){std::vector<int> out;out.reserve(cs.size());for(auto&c:cs){int i=p.acquire();if(i<0)break;auto bots=p.snapshot();c.uploader_alias=bots[i].alias;out.push_back(i);}return out;} }
