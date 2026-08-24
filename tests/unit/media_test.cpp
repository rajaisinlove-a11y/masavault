#include <cassert>
#include "tgcloud/gallery/media.hpp"
int main(){tgcloud::gallery::Catalog c;c.add({"1","/x","x.jpg","image/jpeg","fid",4,1,2,100,200,0,true});assert(c.list(1,0)[0].remote_file_id=="fid");}
