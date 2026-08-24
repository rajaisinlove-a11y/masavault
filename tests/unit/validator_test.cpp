// Task 09 — classify getMe / getChat JSON into valid, invalid_token, inaccessible, malformed.
// Read progress/README.md if you are new to this repo.
#include <cassert>
#include "tgcloud/telegram/validator.hpp"
int main(){using namespace tgcloud::telegram;BotClient c("TEST_BOT_TOKEN");assert(validate_bot(c,R"({"ok":true})").state==Validation::valid);assert(validate_bot(c,R"({"ok":false})").state==Validation::malformed);assert(validate_channel(c,"-1",R"({"ok":true})").state==Validation::valid);}
