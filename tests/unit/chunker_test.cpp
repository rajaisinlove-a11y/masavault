#include <cassert>
#include <fstream>
#include "tgcloud/chunking/chunker.hpp"
int main(){auto d=std::filesystem::temp_directory_path();auto s=d/"tg-in",o=d/"tg-parts";std::ofstream(s,std::ios::binary)<<"abcdef";int n=0;tgcloud::chunking::split(s,o,[&](auto&c,auto&p){assert(c.index==0&&c.total==1&&c.size==6&&!c.sha256.empty());assert(std::filesystem::file_size(p)==6);++n;});assert(n==1);std::filesystem::remove(s);std::filesystem::remove_all(o);}
