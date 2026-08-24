// Task 22 — SYNC1 envelope: PBKDF2-HMAC-SHA256 100k + AES-256-GCM; wrong password fails.
// Read progress/README.md if you are new to this repo.
#include <cassert>
#include "tgcloud/sync/crypto.hpp"
int main(){auto b=tgcloud::sync::encrypt("{\"op\":\"insert\"}","pw");assert(tgcloud::sync::decrypt(b,"pw")=="{\"op\":\"insert\"}");bool bad=false;try{tgcloud::sync::decrypt(b,"bad");}catch(...){bad=true;}assert(bad);}
