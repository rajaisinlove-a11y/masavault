// Whole-project compatibility backtest.
// Offline parts always run. Live Telegram runs when TG_BOT_1..5 and TG_CHAT are set.

#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "tgcloud/backup/backup.hpp"
#include "tgcloud/chunking/chunker.hpp"
#include "tgcloud/database/encrypted_store.hpp"
#include "tgcloud/manifest/link_crypto.hpp"
#include "tgcloud/manifest/link_generator.hpp"
#include "tgcloud/manifest/link_import.hpp"
#include "tgcloud/manifest/manifest.hpp"
#include "tgcloud/scheduler/bot_pool.hpp"
#include "tgcloud/security/redact.hpp"
#include "tgcloud/sync/crypto.hpp"
#include "tgcloud/telegram/bot_client.hpp"
#include "tgcloud/transfer/assignment.hpp"
#include "tgcloud/transfer/reconstruct.hpp"

namespace fs = std::filesystem;

static bool ok_json(const std::string& b) {
    return b.find("\"ok\":true") != std::string::npos || b.find("\"ok\": true") != std::string::npos;
}
static std::string jstr(const std::string& b, const std::string& k) {
    auto pat = "\"" + k + "\":\"";
    auto p = b.find(pat);
    if (p == std::string::npos) return {};
    p += pat.size();
    return b.substr(p, b.find('"', p) - p);
}

int main() {
    int fails = 0;
    auto dir = fs::temp_directory_path() / "masavault-full-backtest";
    fs::remove_all(dir);
    fs::create_directories(dir);

    // Encrypted store keeps chunks
    {
        auto db = dir / "v.tgdb1";
        {
            tgcloud::database::EncryptedFileStore s(db, "pw");
            tgcloud::model::CloudFile f;
            f.name = "a.bin";
            f.chunks.push_back({0, 1, 3, "hhh", 1, "fid", "uid", "bot-1"});
            s.put_file(f);
        }
        tgcloud::database::EncryptedFileStore s(db, "pw");
        if (!s.get_file(1) || s.get_file(1)->chunks.size() != 1) {
            std::cerr << "store-chunks FAIL\n";
            ++fails;
        } else
            std::cout << "store-chunks PASS\n";
    }

    // Sync gzip round-trip
    {
        auto blob = tgcloud::sync::encrypt("{\"op\":\"insert\"}", "pw");
        auto plain = tgcloud::sync::decrypt(blob, "pw");
        if (plain != "{\"op\":\"insert\"}") {
            std::cerr << "sync-gzip FAIL\n";
            ++fails;
        } else
            std::cout << "sync-gzip PASS\n";
    }

    // Redaction
    {
        auto c = tgcloud::security::redact("x=1234567890:AAexampleTokenValueNotReal0001");
        if (c.find("AAexample") != std::string::npos) {
            std::cerr << "redact FAIL\n";
            ++fails;
        } else
            std::cout << "redact PASS\n";
    }

    // Local pipeline
    {
        auto src = dir / "p.bin";
        {
            std::ofstream o(src, std::ios::binary);
            std::string block(4096, 'Z');
            for (int i = 0; i < 1100; ++i) o.write(block.data(), block.size());
        }
        std::vector<tgcloud::model::ChunkRecord> recs;
        tgcloud::chunking::split(src, dir / "parts", [&](const auto& ch, const auto&) {
            recs.push_back({ch.index, ch.total, ch.size, ch.sha256, 0, "", "", ""});
        });
        tgcloud::scheduler::BotPool pool({{"b1"}, {"b2"}, {"b3"}, {"b4"}, {"b5"}});
        tgcloud::transfer::assign_chunks(recs, pool);
        auto out = dir / "out.bin";
        tgcloud::transfer::reconstruct(recs, dir / "parts", out);
        if (fs::file_size(out) != fs::file_size(src)) {
            std::cerr << "local-pipeline FAIL\n";
            ++fails;
        } else
            std::cout << "local-pipeline PASS chunks=" << recs.size() << "\n";
    }

    const char* t1 = std::getenv("TG_BOT_1");
    const char* chat = std::getenv("TG_CHAT");
    if (!t1 || !chat) {
        std::cout << "live SKIP\n";
        fs::remove_all(dir);
        std::cout << "BACKTEST fails=" << fails << "\n";
        return fails ? 1 : 0;
    }
    std::vector<const char*> toks = {t1, std::getenv("TG_BOT_2"), std::getenv("TG_BOT_3"),
                                     std::getenv("TG_BOT_4"), std::getenv("TG_BOT_5")};
    int live_ok = 0;
    for (int i = 0; i < 5; ++i) {
        if (!toks[i]) continue;
        tgcloud::telegram::BotClient c(toks[i]);
        auto me = c.get_me();
        auto ch = c.get_chat(chat);
        bool ok = me.http_status == 200 && ch.http_status == 200 && ok_json(me.body) && ok_json(ch.body);
        std::cout << "live bot" << (i + 1) << " " << (ok ? "PASS" : "FAIL") << "\n";
        if (ok) ++live_ok;
        else ++fails;
    }
    if (live_ok) {
        auto src = dir / "live.bin";
        {
            std::ofstream o(src, std::ios::binary);
            o << "masavault-full-backtest\n";
        }
        tgcloud::telegram::BotClient c(t1);
        auto up = c.send_document(chat, src.string(), "full-backtest");
        bool ok = up.http_status == 200 && ok_json(up.body) && !jstr(up.body, "file_id").empty();
        std::cout << "live upload " << (ok ? "PASS" : "FAIL") << "\n";
        if (!ok) ++fails;
        auto link = dir / "t.link";
        tgcloud::model::CloudFile f;
        f.name = "live.bin";
        tgcloud::manifest::FileManifest mf;
        mf.file = f;
        tgcloud::manifest::generate(mf, "pw", link);
        auto js = tgcloud::manifest::import_link(link, "pw");
        if (js.find("live.bin") == std::string::npos) {
            std::cerr << "live link FAIL\n";
            ++fails;
        } else
            std::cout << "live link PASS\n";
        auto b = tgcloud::backup::encrypt("state", "pw");
        if (tgcloud::backup::decrypt(b, "pw") != "state") {
            std::cerr << "live backup FAIL\n";
            ++fails;
        } else
            std::cout << "live backup PASS\n";
    }
    fs::remove_all(dir);
    std::cout << "BACKTEST fails=" << fails << "\n";
    return fails ? 1 : 0;
}
