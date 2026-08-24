#pragma once
#include <string>
namespace tgcloud::telegram { struct ApiResponse { long http_status{}; std::string body; }; class BotClient { std::string token_; std::string base_; public: explicit BotClient(std::string token,std::string base="https://api.telegram.org"):token_(std::move(token)),base_(std::move(base)){} ApiResponse get_me() const; ApiResponse get_chat(std::string chat_id) const;
 ApiResponse send_document(std::string chat_id, std::string file_path, std::string caption = {}) const;
 ApiResponse get_file(std::string file_id) const;
 ApiResponse download_to(std::string url, std::string destination) const; }; }
