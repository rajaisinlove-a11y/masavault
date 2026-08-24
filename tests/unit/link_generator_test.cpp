#include <cassert>
#include <fstream>
#include "tgcloud/manifest/link_generator.hpp"
int main(){auto p=std::filesystem::temp_directory_path()/"x.link";tgcloud::model::CloudFile f;f.name="a";tgcloud::manifest::generate({"1",f},"pw",p);std::ifstream i(p,std::ios::binary);std::string b((std::istreambuf_iterator<char>(i)),{});assert(tgcloud::manifest::link_decrypt(b,"pw").find("file_name")!=std::string::npos);std::filesystem::remove(p);}
