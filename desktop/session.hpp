#pragma once

#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "tgcloud/database/encrypted_store.hpp"
#include "ui/navigation.hpp"
#include "ui/settings.hpp"
#include "ui/setup.hpp"
#include "ui/transfer_center.hpp"

namespace tgcloud::desktop {

class Session {
    mutable std::mutex mutex_;
    SetupWizard setup_;
    SettingsModel settings_;
    DiagnosticsModel diagnostics_;
    TransferCenterModel transfers_;
    NavigationModel nav_;
    std::vector<std::string> tokens_;
    std::string vault_password_{"local-vault"};
    std::filesystem::path store_path_;
    std::unique_ptr<database::EncryptedFileStore> store_;

public:
    Session();
    std::string status_json() const;
    std::string files_json() const;
    std::string transfers_json() const;
    std::string diagnostics_text() const;
    std::string setup(const std::string& channel, const std::vector<std::string>& tokens);
    std::string upload_bytes(const std::string& name, const std::string& bytes);
    std::string share_selected(const std::vector<std::string>& ids, const std::string& password);
};

}  // namespace tgcloud::desktop
