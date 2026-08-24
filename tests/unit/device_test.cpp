#include <cassert>
#include "tgcloud/sync/device.hpp"
int main(){tgcloud::sync::LogStore s;tgcloud::sync::DeviceSync d("desktop-1",s);tgcloud::model::SyncLog l;l.log_id="x";d.record(l);assert(s.pending()[0].device_id=="desktop-1");d.set_head("node");assert(d.head()=="node");}
