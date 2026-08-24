#include "tgcloud/manifest/link_import.hpp"
#include "tgcloud/manifest/link_crypto.hpp"
#include <fstream>
#include <stdexcept>
namespace tgcloud::manifest {std::string import_link(const std::filesystem::path&p,std::string_view pw){std::ifstream i(p,std::ios::binary);if(!i)throw std::runtime_error("cannot open link file");std::string b((std::istreambuf_iterator<char>(i)),{});return link_decrypt(b,pw);}}
