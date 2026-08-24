#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace tgcloud::desktop {

// Secret-free application preferences. Bot tokens and passwords do not
// belong here — they go to the OS credential store (Task 41 / platform).
struct AppSettings {
    std::string data_directory;
    std::string theme{"system"};  // system | light | dark
    std::size_t max_concurrent_transfers{3};
    bool diagnostics_enabled{true};
};

class SettingsModel {
    AppSettings settings_;

public:
    void set_data_directory(std::string path) { settings_.data_directory = std::move(path); }
    void set_theme(std::string theme) { settings_.theme = std::move(theme); }
    void set_max_concurrent_transfers(std::size_t n) { settings_.max_concurrent_transfers = n; }
    void set_diagnostics_enabled(bool on) { settings_.diagnostics_enabled = on; }

    const AppSettings& get() const { return settings_; }
    bool valid() const;
};

struct DiagnosticLine {
    std::string subsystem;
    std::string level;  // info | warn | error
    std::string message;
};

class DiagnosticsModel {
    std::vector<DiagnosticLine> lines_;

public:
    void add(std::string subsystem, std::string level, std::string message);
    const std::vector<DiagnosticLine>& lines() const { return lines_; }
    std::string export_text() const;
};

}  // namespace tgcloud::desktop
