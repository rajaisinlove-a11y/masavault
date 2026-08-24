#include <cassert>
#include <filesystem>
#include "tgcloud/database/encrypted_store.hpp"
int main(){auto p=std::filesystem::temp_directory_path()/"tgcloud-test.db";std::filesystem::remove(p);{tgcloud::database::EncryptedFileStore s(p,"pw");tgcloud::model::CloudFile f;f.name="secret";f.size_bytes=42;s.put_file(f);} {tgcloud::database::EncryptedFileStore s(p,"pw");assert(s.get_file(1)->name=="secret");} bool bad=false;try{tgcloud::database::EncryptedFileStore x(p,"bad");}catch(...){bad=true;}assert(bad);std::filesystem::remove(p);}
