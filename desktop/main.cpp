#include <atomic>
#include <iostream>
#include <string>

#include "http_server.hpp"
#include "session.hpp"
#include "tgcloud/core.hpp"

int main(int argc, char** argv) {
    int port = 8787;
    std::string www = "desktop/www";
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--name") {
            std::cout << tgcloud::product_name() << "\n";
            return 0;
        }
        if (a == "--port" && i + 1 < argc) port = std::stoi(argv[++i]);
        if (a == "--www" && i + 1 < argc) www = argv[++i];
    }
    tgcloud::desktop::Session session;
    std::cout << tgcloud::product_name() << " listening on 0.0.0.0:" << port << "\n";
    std::cout.flush();
    std::atomic<bool> running{true};
    tgcloud::desktop::serve_forever("0.0.0.0", port, www, session, &running);
    return 0;
}
