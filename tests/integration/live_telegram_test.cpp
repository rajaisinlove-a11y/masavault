// Task 40 — optional live Telegram check.
//
// Default: SKIP (exit 0) so CTest stays secret-free and offline.
// Set TG_BOT_1 and TG_CHAT to actually call getMe + getChat.

#include <cstdlib>
#include <iostream>
#include <string>

#include "tgcloud/telegram/bot_client.hpp"

static bool ok_body(const std::string& body) {
    return body.find("\"ok\":true") != std::string::npos || body.find("\"ok\": true") != std::string::npos;
}

int main() {
    const char* token = std::getenv("TG_BOT_1");
    const char* chat = std::getenv("TG_CHAT");
    if (!token || !chat || !*token || !*chat) {
        std::cout << "live_telegram_test SKIP (TG_BOT_1 / TG_CHAT not set)\n";
        return 0;
    }
    tgcloud::telegram::BotClient client(token);
    auto me = client.get_me();
    auto ch = client.get_chat(chat);
    const bool pass = me.http_status == 200 && ch.http_status == 200 && ok_body(me.body) && ok_body(ch.body);
    std::cout << "live_telegram_test getMe=" << me.http_status << " getChat=" << ch.http_status
              << (pass ? " PASS\n" : " FAIL\n");
    return pass ? 0 : 1;
}
