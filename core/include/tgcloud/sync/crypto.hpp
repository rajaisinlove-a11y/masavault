#pragma once
#include <string>
namespace tgcloud::sync { std::string encrypt(std::string_view json,std::string_view password); std::string decrypt(std::string_view blob,std::string_view password); }
