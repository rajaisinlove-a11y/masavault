#include "tgcloud/sync/chain.hpp"
#include <algorithm>
#include <unordered_map>
#include <stdexcept>
namespace tgcloud::sync {std::vector<Node> traverse(const std::vector<Node>&ns,std::string head){std::unordered_map<std::string,const Node*> m;for(auto&n:ns)m[n.id]=&n;std::vector<Node> out;std::string cur=head;while(!cur.empty()){auto it=m.find(cur);if(it==m.end())throw std::runtime_error("broken sync chain");out.push_back(*it->second);cur=it->second->prev_id;if(out.size()>ns.size())throw std::runtime_error("sync chain cycle");}std::reverse(out.begin(),out.end());return out;}}
