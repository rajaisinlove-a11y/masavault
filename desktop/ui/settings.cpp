#include "settings.hpp"

#include "tgcloud/security/redact.hpp"

namespace tgcloud::desktop {

bool SettingsModel::valid() const {
    if (settings_.theme != "system" && settings_.theme != "light" && settings_.theme != "dark")
        return false;
    if (settings_.max_concurrent_transfers == 0 || settings_.max_concurrent_transfers > 16)
        return false;
    return true;
}

void DiagnosticsModel::add(std::string subsystem, std::string level, std::string message) {
    DiagnosticLine line;
    line.subsystem = std::move(subsystem);
    line.level = std::move(level);
    line.message = tgcloud::security::redact(message);
    lines_.push_back(std::move(line));
}

std::string DiagnosticsModel::export_text() const {
    std::string out;
    out += "TG Cloud Desktop diagnostics (secrets redacted)\n";
    for (const auto& line : lines_) {
        out += line.level;
        out += " [";
        out += line.subsystem;
        out += "] ";
        out += line.message;
        out += "\n";
    }
    return tgcloud::security::redact(out);
}

}  // namespace tgcloud::desktop
