// Task 27 — .link crypto: PBKDF2 10k + AES-256-CBC, salt||iv||ciphertext.
// Read progress/README.md if you are new to this repo.
#include <cassert>
#include "tgcloud/manifest/link_crypto.hpp"
int main(){auto b=tgcloud::manifest::link_encrypt("{\"version\":\"1\"}","pw");assert(b.size()>32&&tgcloud::manifest::link_decrypt(b,"pw")=="{\"version\":\"1\"}");bool bad=false;try{tgcloud::manifest::link_decrypt(b,"bad");}catch(...){bad=true;}assert(bad);}
