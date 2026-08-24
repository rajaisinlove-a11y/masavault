#include "tgcloud/chunking/chunker.hpp"
#include <fstream>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#include <stdexcept>
namespace tgcloud::chunking { std::uint32_t count(std::uint64_t n){return n?static_cast<std::uint32_t>((n+compatible_chunk_size-1)/compatible_chunk_size):0;} static std::string hash(const std::filesystem::path&p){std::ifstream f(p,std::ios::binary);SHA256_CTX c;SHA256_Init(&c);char b[65536];while(f.read(b,sizeof b)||f.gcount())SHA256_Update(&c,b,f.gcount());unsigned char d[SHA256_DIGEST_LENGTH];SHA256_Final(d,&c);std::ostringstream o;for(auto x:d)o<<std::hex<<std::setw(2)<<std::setfill('0')<<(int)x;return o.str();} void split(const std::filesystem::path&src,const std::filesystem::path&dir,ChunkConsumer cb){auto size=std::filesystem::file_size(src);auto total=count(size);std::filesystem::create_directories(dir);std::ifstream in(src,std::ios::binary);for(std::uint32_t i=0;i<total;i++){auto p=dir/(std::to_string(i)+".part");std::ofstream out(p,std::ios::binary);std::uint64_t want=std::min(compatible_chunk_size,size-(std::uint64_t)i*compatible_chunk_size);char b[65536];while(want){auto n=std::min<std::uint64_t>(want,sizeof b);in.read(b,n);if((std::uint64_t)in.gcount()!=n)throw std::runtime_error("short input");out.write(b,n);want-=n;}out.close();cb({i,total,std::filesystem::file_size(p),hash(p)},p);}} }
