#include <cassert>
#include "tgcloud/sync/node.hpp"
int main(){tgcloud::sync::Node n{"n2","n1",4,{}};tgcloud::model::SyncLog l;l.log_id="l";l.table_name="cloud_files";l.primary_key="1";l.data_json="{}";n.entries.push_back(l);auto j=tgcloud::sync::node_json(n);assert(j.find("prevId")!=std::string::npos);assert(tgcloud::sync::decrypt(tgcloud::sync::encrypt_node(n,"pw"),"pw")==j);}
