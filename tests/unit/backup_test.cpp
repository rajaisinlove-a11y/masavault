#include <cassert>
#include "tgcloud/backup/backup.hpp"
int main(){auto b=tgcloud::backup::encrypt("backup_manifest", "pw");assert(tgcloud::backup::decrypt(b,"pw")=="backup_manifest");bool bad=false;try{tgcloud::backup::decrypt(b,"bad");}catch(...){bad=true;}assert(bad);}
