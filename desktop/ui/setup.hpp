#pragma once
#include <string>
#include <vector>
namespace tgcloud::desktop { struct SetupConfig{std::string storage_channel,sync_channel,device_id;std::vector<std::string> bot_aliases;bool sync_enabled{false};}; class SetupWizard{SetupConfig c_;public:void set_storage_channel(std::string v){c_.storage_channel=std::move(v);}void set_sync_channel(std::string v){c_.sync_channel=std::move(v);}void add_bot_alias(std::string v){c_.bot_aliases.push_back(std::move(v));}const SetupConfig& config()const{return c_;}}; }
