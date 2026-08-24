#include <cassert>
#include "tgcloud/transfer/control.hpp"
int main(){tgcloud::transfer::Cancellation c;assert(!c.is_cancelled());c.cancel();assert(c.is_cancelled());tgcloud::transfer::Error e{tgcloud::transfer::ErrorKind::timeout,"request timed out"};assert(e.kind==tgcloud::transfer::ErrorKind::timeout);}
