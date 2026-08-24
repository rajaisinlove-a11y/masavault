// Task 39 — Settings, security, and diagnostics
//
// 1. Preferences never hold a bot token.
// 2. Invalid theme / concurrency is rejected.
// 3. A token pasted into a log line is replaced with [REDACTED].
// 4. The .link share warning is the required security text.

#include <cassert>
#include <string>

#include "desktop/ui/settings.hpp"
#include "tgcloud/security/redact.hpp"

int main() {
    tgcloud::desktop::SettingsModel settings;
    settings.set_theme("dark");
    settings.set_max_concurrent_transfers(4);
    settings.set_data_directory("/home/user/.local/share/masavault");
    assert(settings.valid());
    assert(settings.get().theme == "dark");

    settings.set_theme("rainbow");
    assert(!settings.valid());
    settings.set_theme("light");
    settings.set_max_concurrent_transfers(0);
    assert(!settings.valid());
    settings.set_max_concurrent_transfers(3);
    assert(settings.valid());

    // Sample shape only — not a real credential.
    const std::string sample = "1234567890:AAexampleTokenValueNotReal0001";
    assert(tgcloud::security::looks_like_bot_token(sample));
    const std::string cleaned = tgcloud::security::redact("bot=" + sample + " ok");
    assert(cleaned.find("[REDACTED]") != std::string::npos);
    assert(cleaned.find("AAexample") == std::string::npos);

    tgcloud::desktop::DiagnosticsModel diag;
    diag.add("telegram", "error", std::string("getMe failed token=") + sample);
    const std::string report = diag.export_text();
    assert(report.find("[REDACTED]") != std::string::npos);
    assert(report.find("AAexample") == std::string::npos);
    assert(report.find("telegram") != std::string::npos);

    const auto warning = tgcloud::security::link_share_warning();
    assert(warning.find("Telegram") != std::string::npos);
    assert(warning.find("password") != std::string::npos);
    return 0;
}
