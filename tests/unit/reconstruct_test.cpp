// Task 16 — stitch part files in logical index order; missing/duplicate indexes fail.
// Read progress/README.md if you are new to this repo.
#include <cassert>
#include <fstream>
#include "tgcloud/transfer/reconstruct.hpp"
int main(){auto d=std::filesystem::temp_directory_path()/"tg-parts";std::filesystem::create_directories(d);std::ofstream(d/"0.part")<<"ab";std::ofstream(d/"1.part")<<"cd";std::vector<tgcloud::model::ChunkRecord>c={{1,2,2},{0,2,2}};auto o=d/"out";tgcloud::transfer::reconstruct(c,d,o);std::ifstream in(o);std::string s;in>>s;assert(s=="abcd");std::filesystem::remove_all(d);}
