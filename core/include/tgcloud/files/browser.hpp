#pragma once
#include <string>
#include <vector>
namespace tgcloud::files { struct Entry{std::string id,name,mime;bool folder;}; class BrowserBackend{std::string scheme_{"tgcloud"};public:std::string uri(std::string_view id)const{return scheme_+"://"+std::string(id);}std::vector<Entry> children(std::string_view)const{return {};}}; }
