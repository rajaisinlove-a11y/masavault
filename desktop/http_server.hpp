#pragma once
#include <atomic>
#include <string>
#include "session.hpp"

namespace tgcloud::desktop {
// Bind 0.0.0.0 so the Arena live preview can reach the desktop shell.
void serve_forever(const std::string& host, int port, const std::string& www, Session& session,
                   std::atomic<bool>* running);
}  // namespace tgcloud::desktop
