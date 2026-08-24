#pragma once
#include <string>
namespace tgcloud::backup { std::string encrypt(std::string_view payload,std::string_view password); std::string decrypt(std::string_view blob,std::string_view password); }
