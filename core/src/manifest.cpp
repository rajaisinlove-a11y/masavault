#include "tgcloud/manifest/manifest.hpp"

#include <cstdlib>
#include <sstream>
#include <string_view>

namespace tgcloud::manifest {

static std::string q(const std::string& s) {
    std::string o = "\"";
    for (char c : s) {
        if (c == '"' || c == '\\') o += '\\';
        o += c;
    }
    return o + "\"";
}

std::string to_json(const FileManifest& m) {
    std::ostringstream o;
    o << "{\"version\":" << q(m.version) << ",\"type\":\"file\",\"file_name\":" << q(m.file.name)
      << ",\"file_size\":" << m.file.size_bytes << ",\"mime_type\":" << q(m.file.mime_type)
      << ",\"chunks\":[";
    for (size_t i = 0; i < m.file.chunks.size(); ++i) {
        const auto& c = m.file.chunks[i];
        if (i) o << ',';
        o << "{\"chunk_number\":" << c.index << ",\"total_chunks\":" << c.total
          << ",\"chunk_size\":" << c.size << ",\"chunk_hash\":" << q(c.sha256)
          << ",\"telegram_file_id\":" << q(c.telegram_file_id) << ",\"uploader\":" << q(c.uploader_alias)
          << "}";
    }
    o << "]}";
    return o.str();
}

static std::string str_field(std::string_view j, const char* key) {
    std::string pat = std::string("\"") + key + "\":\"";
    auto p = j.find(pat);
    if (p == std::string_view::npos) return {};
    p += pat.size();
    std::string o;
    for (size_t i = p; i < j.size(); ++i) {
        if (j[i] == '\\' && i + 1 < j.size()) {
            o += j[++i];
            continue;
        }
        if (j[i] == '"') break;
        o += j[i];
    }
    return o;
}

static std::uint64_t num_field(std::string_view j, const char* key) {
    std::string pat = std::string("\"") + key + "\":";
    auto p = j.find(pat);
    if (p == std::string_view::npos) return 0;
    p += pat.size();
    return std::strtoull(j.data() + p, nullptr, 10);
}

FileManifest from_json(std::string_view j) {
    FileManifest m;
    m.version = str_field(j, "version");
    m.file.name = str_field(j, "file_name");
    m.file.mime_type = str_field(j, "mime_type");
    m.file.size_bytes = num_field(j, "file_size");
    auto chunks_at = j.find("\"chunks\":");
    if (chunks_at == std::string_view::npos) return m;
    size_t i = j.find('[', chunks_at);
    if (i == std::string_view::npos) return m;
    ++i;
    while (i < j.size()) {
        auto obj = j.find('{', i);
        if (obj == std::string_view::npos) break;
        auto end = j.find('}', obj);
        if (end == std::string_view::npos) break;
        auto part = j.substr(obj, end - obj + 1);
        model::ChunkRecord c;
        c.index = static_cast<std::uint32_t>(num_field(part, "chunk_number"));
        c.total = static_cast<std::uint32_t>(num_field(part, "total_chunks"));
        c.size = num_field(part, "chunk_size");
        c.sha256 = str_field(part, "chunk_hash");
        c.telegram_file_id = str_field(part, "telegram_file_id");
        c.uploader_alias = str_field(part, "uploader");
        m.file.chunks.push_back(std::move(c));
        i = end + 1;
    }
    return m;
}

}  // namespace tgcloud::manifest
