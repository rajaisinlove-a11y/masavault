#include "tgcloud/manifest/link_generator.hpp"
#include "tgcloud/manifest/link_crypto.hpp"
#include <fstream>
namespace tgcloud::manifest {static void write(const std::string&s,const std::filesystem::path&p){std::ofstream o(p,std::ios::binary|std::ios::trunc);o.write(s.data(),s.size());}void generate(const FileManifest&m,std::string_view pw,const std::filesystem::path&p){write(link_encrypt(to_json(m),pw),p);}void generate_batch(const std::vector<FileManifest>&ms,std::string_view pw,const std::filesystem::path&p){std::string j="{\"version\":\"1\",\"type\":\"batch\",\"files\":[";for(size_t i=0;i<ms.size();++i){if(i)j+=',';j+=to_json(ms[i]);}j+="]}";write(link_encrypt(j,pw),p);}}
