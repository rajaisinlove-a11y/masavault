#include "tgcloud/transfer/reconstruct.hpp"
#include <algorithm>
#include <fstream>
#include <stdexcept>
namespace tgcloud::transfer { void reconstruct(const std::vector<model::ChunkRecord>&cs,const std::filesystem::path&dir,const std::filesystem::path&outp){std::vector<const model::ChunkRecord*> v;for(auto&c:cs)v.push_back(&c);std::sort(v.begin(),v.end(),[](auto*a,auto*b){return a->index<b->index;});std::ofstream out(outp,std::ios::binary|std::ios::trunc);for(size_t i=0;i<v.size();++i){if(v[i]->index!=i)throw std::runtime_error("missing or duplicate chunk");std::ifstream in(dir/(std::to_string(i)+".part"),std::ios::binary);if(!in)throw std::runtime_error("missing chunk file");out<<in.rdbuf();}}}
