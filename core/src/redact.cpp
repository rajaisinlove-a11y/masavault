#include "tgcloud/security/redact.hpp"

#include <cctype>
#include <sstream>

namespace tgcloud::security {
namespace {

bool is_token_char(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '-';
}

}  // namespace

bool looks_like_bot_token(std::string_view text) {
    // id:secret  — id is 8–12 digits, secret starts with AA and is long.
    auto colon = text.find(':');
    if (colon == std::string_view::npos || colon < 8 || colon > 12) return false;
    for (size_t i = 0; i < colon; ++i) {
        if (!std::isdigit(static_cast<unsigned char>(text[i]))) return false;
    }
    if (colon + 3 >= text.size()) return false;
    if (text[colon + 1] != 'A' || text[colon + 2] != 'A') return false;
    if (text.size() - colon < 20) return false;
    return true;
}

bool looks_like_secret(std::string_view text) {
    if (looks_like_bot_token(text)) return true;
    if (text.find("password") != std::string_view::npos) return true;
    if (text.find("token") != std::string_view::npos && text.find(':') != std::string_view::npos)
        return true;
    return false;
}

std::string redact(std::string_view text) {
    std::string out;
    out.reserve(text.size());
    for (size_t i = 0; i < text.size();) {
        // Digit run that might start a bot token.
        if (std::isdigit(static_cast<unsigned char>(text[i]))) {
            size_t j = i;
            while (j < text.size() && std::isdigit(static_cast<unsigned char>(text[j]))) ++j;
            if (j < text.size() && text[j] == ':' && j - i >= 8 && j - i <= 12) {
                size_t k = j + 1;
                while (k < text.size() && is_token_char(text[k])) ++k;
                std::string_view cand = text.substr(i, k - i);
                if (looks_like_bot_token(cand)) {
                    out += "[REDACTED]";
                    i = k;
                    continue;
                }
            }
        }
        out += text[i];
        ++i;
    }
    return out;
}

std::string link_share_warning() {
    return "Shared manifests may contain sensitive Telegram access information. "
           "Protect the share password.";
}

}  // namespace tgcloud::security
