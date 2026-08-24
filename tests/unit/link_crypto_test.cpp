#include <cassert>
#include "tgcloud/manifest/link_crypto.hpp"
int main(){auto b=tgcloud::manifest::link_encrypt("{\"version\":\"1\"}","pw");assert(b.size()>32&&tgcloud::manifest::link_decrypt(b,"pw")=="{\"version\":\"1\"}");bool bad=false;try{tgcloud::manifest::link_decrypt(b,"bad");}catch(...){bad=true;}assert(bad);}
