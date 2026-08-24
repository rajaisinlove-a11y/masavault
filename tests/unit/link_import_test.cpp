#include <cassert>
#include <fstream>
#include "tgcloud/manifest/link_import.hpp"
#include "tgcloud/manifest/link_crypto.hpp"
int main(){auto p=std::filesystem::temp_directory_path()/"i.link";std::ofstream(p,std::ios::binary)<<tgcloud::manifest::link_encrypt("{\"files\":[]}","pw");assert(tgcloud::manifest::import_link(p,"pw").find("files")!=std::string::npos);std::filesystem::remove(p);}
