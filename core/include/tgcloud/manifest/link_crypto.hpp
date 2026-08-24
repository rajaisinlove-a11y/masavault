#pragma once
#include <string>
namespace tgcloud::manifest { std::string link_encrypt(std::string_view json,std::string_view password); std::string link_decrypt(std::string_view blob,std::string_view password); }
