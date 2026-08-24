// Task 30 — BKP1: SHA-256(password||salt) + AES-256-CBC; bad password fails.
// Read progress/README.md if you are new to this repo.
#include <cassert>
#include "tgcloud/backup/backup.hpp"
int main(){auto b=tgcloud::backup::encrypt("backup_manifest", "pw");assert(tgcloud::backup::decrypt(b,"pw")=="backup_manifest");bool bad=false;try{tgcloud::backup::decrypt(b,"bad");}catch(...){bad=true;}assert(bad);}
