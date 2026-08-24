#include "tgcloud/sync/node.hpp"
#include "tgcloud/sync/crypto.hpp"
#include <sstream>
namespace tgcloud::sync {static std::string q(const std::string&s){return "\""+s+"\"";}std::string node_json(const Node&n){std::ostringstream o;o<<"{\"id\":"<<q(n.id)<<",\"prevId\":"<<q(n.prev_id)<<",\"timestamp\":"<<n.timestamp<<",\"entries\":[";for(size_t i=0;i<n.entries.size();++i){if(i)o<<',';auto&e=n.entries[i];o<<"{\"logId\":"<<q(e.log_id)<<",\"operation\":\"insert\",\"table\":"<<q(e.table_name)<<",\"primaryKey\":"<<q(e.primary_key)<<",\"data\":"<<q(e.data_json)<<"}";}o<<"]}";return o.str();}std::string encrypt_node(const Node&n,std::string_view p){return encrypt(node_json(n),p);}}
