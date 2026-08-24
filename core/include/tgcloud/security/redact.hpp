#pragma once

#include <string>
#include <string_view>

namespace tgcloud::security {

// Telegram bot tokens look like: 123456789:AA....  They must never appear
// in settings files, diagnostics, or logs.
bool looks_like_bot_token(std::string_view text);
bool looks_like_secret(std::string_view text);

// Replace bot tokens and obvious password= / token= values with [REDACTED].
std::string redact(std::string_view text);

// Shown whenever the user creates or opens a .link share.
std::string link_share_warning();

}  // namespace tgcloud::security
