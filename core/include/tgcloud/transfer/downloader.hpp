#pragma once
#include <filesystem>
#include "tgcloud/telegram/bot_client.hpp"
namespace tgcloud::transfer { class Downloader { const telegram::BotClient& client_; public: explicit Downloader(const telegram::BotClient& c):client_(c){} telegram::ApiResponse fetch(std::string url,const std::filesystem::path& dest)const{return client_.download_to(std::move(url),dest.string());} }; }
