#include <cassert>
#include "tgcloud/sync/crypto.hpp"
int main(){auto b=tgcloud::sync::encrypt("{\"op\":\"insert\"}","pw");assert(tgcloud::sync::decrypt(b,"pw")=="{\"op\":\"insert\"}");bool bad=false;try{tgcloud::sync::decrypt(b,"bad");}catch(...){bad=true;}assert(bad);}
