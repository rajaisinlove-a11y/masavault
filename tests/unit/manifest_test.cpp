// Task 12 — FileManifest JSON keeps chunk index, hash, Telegram ids, uploader alias.
// Read progress/README.md if you are new to this repo.
#include <cassert>
#include "tgcloud/manifest/manifest.hpp"
int main(){tgcloud::model::CloudFile f;f.name="x.txt";f.size_bytes=3;f.chunks.push_back({0,1,3,"abc",7,"fid","uid","bot1"});auto j=tgcloud::manifest::to_json({"1",f});assert(j.find("chunk_number")!=std::string::npos&&j.find("bot1")!=std::string::npos);}
