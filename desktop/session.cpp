#include "session.hpp"

#include <chrono>
#include <fstream>
#include <sstream>

#include "tgcloud/chunking/chunker.hpp"
#include "tgcloud/manifest/link_generator.hpp"
#include "tgcloud/manifest/manifest.hpp"
#include "tgcloud/scheduler/bot_pool.hpp"
#include "tgcloud/security/redact.hpp"
#include "tgcloud/telegram/bot_client.hpp"
#include "tgcloud/transfer/assignment.hpp"
#include "tgcloud/transfer/progress.hpp"

namespace tgcloud::desktop {
namespace {

std::string json_escape(const std::string& s) {
    std::string o;
    o.reserve(s.size());
    for (char c : s) {
        if (c == '"' || c == '\\') o += '\\';
        if (c == '\n') {
            o += "\\n";
            continue;
        }
        o += c;
    }
    return o;
}

std::string json_string_field(const std::string& body, const std::string& key) {
    auto pat = "\"" + key + "\":\"";
    auto pos = body.find(pat);
    if (pos == std::string::npos) return {};
    pos += pat.size();
    auto end = body.find('"', pos);
    if (end == std::string::npos) return {};
    return body.substr(pos, end - pos);
}

bool json_ok(const std::string& body) {
    return body.find("\"ok\":true") != std::string::npos || body.find("\"ok\": true") != std::string::npos;
}

}  // namespace

Session::Session() {
    auto dir = std::filesystem::temp_directory_path() / "masavault-session";
    std::filesystem::create_directories(dir);
    store_path_ = dir / "vault.tgdb1";
    store_ = std::make_unique<database::EncryptedFileStore>(store_path_, vault_password_);
    settings_.set_data_directory(dir.string());
    settings_.set_theme("dark");
}

std::string Session::status_json() const {
    std::lock_guard lock(mutex_);
    auto files = store_->list_files(10000, 0);
    std::uint64_t bytes = 0;
    for (auto& f : files) bytes += f.size_bytes;
    std::ostringstream o;
    o << "{\"product\":\"TG Cloud Desktop\",\"page\":\""
      << (nav_.current() == Page::settings ? "settings" : "cloud")
      << "\",\"file_count\":" << files.size() << ",\"logical_bytes\":" << bytes
      << ",\"bots_configured\":" << tokens_.size() << ",\"channel\":\""
      << json_escape(setup_.config().storage_channel) << "\",\"ready\":"
      << (tokens_.empty() || setup_.config().storage_channel.empty() ? "false" : "true") << "}";
    return o.str();
}

std::string Session::files_json() const {
    std::lock_guard lock(mutex_);
    auto files = store_->list_files(10000, 0);
    std::ostringstream o;
    o << "[";
    for (size_t i = 0; i < files.size(); ++i) {
        if (i) o << ',';
        o << "{\"id\":" << files[i].local_id << ",\"name\":\"" << json_escape(files[i].name)
          << "\",\"size\":" << files[i].size_bytes << ",\"chunks\":" << files[i].chunks.size()
          << ",\"telegram_file_id\":\"" << json_escape(files[i].telegram_file_id) << "\"}";
    }
    o << "]";
    return o.str();
}

std::string Session::transfers_json() const {
    std::lock_guard lock(mutex_);
    std::ostringstream o;
    o << "[";
    auto rows = transfers_.rows();
    for (size_t i = 0; i < rows.size(); ++i) {
        if (i) o << ',';
        o << "{\"name\":\"" << json_escape(rows[i].task.display_name) << "\",\"completed\":"
          << rows[i].progress.completed << ",\"total\":" << rows[i].progress.total << "}";
    }
    o << "]";
    return o.str();
}

std::string Session::diagnostics_text() const {
    std::lock_guard lock(mutex_);
    return diagnostics_.export_text();
}

std::string Session::setup(const std::string& channel, const std::vector<std::string>& tokens) {
    std::lock_guard lock(mutex_);
    setup_.set_storage_channel(channel);
    tokens_.clear();
    int ok = 0;
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i].empty()) continue;
        telegram::BotClient client(tokens[i]);
        auto me = client.get_me();
        auto ch = client.get_chat(channel);
        bool pass = me.http_status == 200 && ch.http_status == 200 && json_ok(me.body) && json_ok(ch.body);
        diagnostics_.add("telegram", pass ? "info" : "error",
                         std::string("bot ") + std::to_string(i + 1) + " getMe/getChat " +
                             (pass ? "PASS" : "FAIL") + " token=" + tokens[i]);
        if (pass) {
            tokens_.push_back(tokens[i]);
            setup_.add_bot_alias(std::string("bot-") + std::to_string(tokens_.size()));
            ++ok;
        }
    }
    std::ostringstream o;
    o << "{\"ok\":" << (ok ? "true" : "false") << ",\"bots\":" << ok << "}";
    return o.str();
}

std::string Session::upload_bytes(const std::string& name, const std::string& bytes) {
    std::lock_guard lock(mutex_);
    if (tokens_.empty() || setup_.config().storage_channel.empty())
        return "{\"ok\":false,\"error\":\"configure bots and channel first\"}";
    auto dir = std::filesystem::temp_directory_path() / "masavault-upload";
    std::filesystem::create_directories(dir);
    auto src = dir / name;
    {
        std::ofstream o(src, std::ios::binary);
        o.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    }
    auto parts = dir / (name + ".parts");
    std::vector<model::ChunkRecord> recs;
    chunking::split(src, parts, [&](const chunking::Chunk& ch, const std::filesystem::path&) {
        model::ChunkRecord r;
        r.index = ch.index;
        r.total = ch.total;
        r.size = ch.size;
        r.sha256 = ch.sha256;
        recs.push_back(r);
    });
    std::vector<scheduler::Bot> bots;
    for (size_t i = 0; i < tokens_.size(); ++i) bots.push_back({std::string("bot-") + std::to_string(i + 1)});
    scheduler::BotPool pool(std::move(bots));
    transfer::assign_chunks(recs, pool);

    model::CloudFile file;
    file.name = name;
    file.size_bytes = bytes.size();
    file.uploaded_at = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    for (auto& rec : recs) {
        int bi = 0;
        if (rec.uploader_alias.size() > 4) bi = rec.uploader_alias.back() - '1';
        if (bi < 0 || bi >= static_cast<int>(tokens_.size())) bi = 0;
        telegram::BotClient client(tokens_[static_cast<size_t>(bi)]);
        auto part = parts / (std::to_string(rec.index) + ".part");
        auto up = client.send_document(setup_.config().storage_channel, part.string(),
                                       "chunk " + std::to_string(rec.index));
        if (up.http_status != 200 || !json_ok(up.body)) {
            diagnostics_.add("transfer", "error", "upload failed for " + name);
            return "{\"ok\":false,\"error\":\"telegram upload failed\"}";
        }
        rec.telegram_file_id = json_string_field(up.body, "file_id");
        file.telegram_file_id = rec.telegram_file_id;
    }
    file.chunks = recs;
    store_->put_file(file);
    TransferRow row;
    row.task.display_name = name;
    row.task.size_bytes = bytes.size();
    row.progress.completed = bytes.size();
    row.progress.total = bytes.size();
    transfers_.add(row);
    diagnostics_.add("transfer", "info", "uploaded " + name);
    std::ostringstream o;
    o << "{\"ok\":true,\"name\":\"" << json_escape(name) << "\",\"chunks\":" << recs.size() << "}";
    return o.str();
}

std::string Session::share_selected(const std::vector<std::string>& ids, const std::string& password) {
    std::lock_guard lock(mutex_);
    std::vector<manifest::FileManifest> batch;
    for (auto& id : ids) {
        auto f = store_->get_file(std::stoll(id));
        if (!f) continue;
        batch.push_back({"1", *f});
    }
    if (batch.empty()) return "{\"ok\":false,\"error\":\"no files\"}";
    auto path = std::filesystem::temp_directory_path() / "masavault-share.link";
    if (batch.size() == 1)
        manifest::generate(batch[0], password, path);
    else
        manifest::generate_batch(batch, password, path);
    diagnostics_.add("share", "info", security::link_share_warning());
    return std::string("{\"ok\":true,\"path\":\"") + json_escape(path.string()) +
           "\",\"warning\":\"" + json_escape(security::link_share_warning()) + "\"}";
}

}  // namespace tgcloud::desktop
