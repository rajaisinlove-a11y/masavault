#pragma once
#include <string>
#include "bot_client.hpp"
namespace tgcloud::telegram { enum class Validation { valid, invalid_token, inaccessible, malformed }; struct ValidationResult { Validation state; std::string detail; }; ValidationResult validate_bot(BotClient&, std::string_view response); ValidationResult validate_channel(BotClient&, std::string chat_id, std::string_view response); }
