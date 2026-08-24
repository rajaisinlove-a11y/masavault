// Task 40 — compatibility matrix against SYNTHETIC fixtures.
//
// These tests prove our own adapters keep the recovered field names.
// They do NOT prove byte-identity with a real Android .link / sync-node /
// 252-chunk backup (those artifacts stay in apkforensic).

#include <cassert>
#include <filesystem>
#include <fstream>
#include <set>
#include <string>

#include "tgcloud/backup/backup.hpp"
#include "tgcloud/compat/cloud_files.hpp"
#include "tgcloud/database/memory_store.hpp"
#include "tgcloud/manifest/link_crypto.hpp"
#include "tgcloud/manifest/link_generator.hpp"
#include "tgcloud/manifest/link_import.hpp"
#include "tgcloud/manifest/manifest.hpp"
#include "tgcloud/streaming/range.hpp"
#include "tgcloud/sync/chain.hpp"
#include "tgcloud/sync/conflict.hpp"
#include "tgcloud/sync/crypto.hpp"
#include "tgcloud/sync/node.hpp"

static std::string slurp(const std::filesystem::path& p) {
    std::ifstream in(p);
    return std::string((std::istreambuf_iterator<char>(in)), {});
}

int main() {
    // --- synthetic 2-chunk share fixture ---
    const auto share_path = std::filesystem::path("fixtures/synthetic-share.json");
    const auto fixture_json = slurp(share_path);
    auto parsed = tgcloud::manifest::from_json(fixture_json);
    assert(parsed.file.name == "clip.mp4");
    assert(parsed.file.chunks.size() == 2);
    assert(parsed.file.chunks[0].index == 0);
    assert(parsed.file.chunks[1].index == 1);
    assert(parsed.file.chunks[0].uploader_alias == "bot-a");
    assert(parsed.file.chunks[1].telegram_file_id == "file-1");

    auto again = tgcloud::manifest::from_json(tgcloud::manifest::to_json(parsed));
    assert(again.file.chunks.size() == 2);
    assert(again.file.chunks[1].uploader_alias == "bot-b");

    // --- 252-chunk synthetic manifest (generated, not stored) ---
    tgcloud::manifest::FileManifest big;
    big.file.name = "video.mp4";
    big.file.size_bytes = 252ULL * 4ULL * 1024ULL * 1024ULL;
    big.file.mime_type = "video/mp4";
    for (std::uint32_t i = 0; i < 252; ++i) {
        tgcloud::model::ChunkRecord c;
        c.index = i;
        c.total = 252;
        c.size = 4ULL * 1024ULL * 1024ULL;
        c.sha256 = "h" + std::to_string(i);
        c.telegram_file_id = "tg-" + std::to_string(i);
        c.uploader_alias = "bot-" + std::to_string(i % 5);
        big.file.chunks.push_back(c);
    }
    auto big_json = tgcloud::manifest::to_json(big);
    auto big_parsed = tgcloud::manifest::from_json(big_json);
    assert(big_parsed.file.chunks.size() == 252);
    assert(big_parsed.file.chunks.front().index == 0);
    assert(big_parsed.file.chunks.back().index == 251);
    std::set<std::string> bots;
    for (const auto& c : big_parsed.file.chunks) bots.insert(c.uploader_alias);
    assert(bots.size() == 5);

    // --- .link generate / import round-trip ---
    auto link_path = std::filesystem::temp_directory_path() / "matrix.link";
    tgcloud::manifest::generate(parsed, "share-pw", link_path);
    auto imported = tgcloud::manifest::import_link(link_path, "share-pw");
    assert(imported.find("clip.mp4") != std::string::npos);
    assert(imported.find("file-0") != std::string::npos);
    std::filesystem::remove(link_path);

    // --- sync node + chain + one-entry INSERT ---
    tgcloud::sync::Node a{"node-a", "", 1, {}};
    tgcloud::model::SyncLog log;
    log.log_id = "log-1";
    log.table_name = "cloud_files";
    log.primary_key = "uuid-1";
    log.data_json = "{\"name\":\"clip.mp4\"}";
    tgcloud::sync::Node b{"node-b", "node-a", 2, {log}};
    auto blob = tgcloud::sync::encrypt_node(b, "sync-pw");
    auto plain = tgcloud::sync::decrypt(blob, "sync-pw");
    assert(plain.find("node-b") != std::string::npos);
    assert(plain.find("cloud_files") != std::string::npos);
    auto chain = tgcloud::sync::traverse({a, b}, "node-b");
    assert(chain.size() == 2);
    assert(chain[0].id == "node-a");
    assert(chain[1].entries.size() == 1);

    // --- conflict merge ---
    tgcloud::model::SyncLog local = log;
    local.timestamp = 10;
    local.data_json = "{\"name\":\"old\"}";
    tgcloud::model::SyncLog remote = log;
    remote.timestamp = 20;
    remote.data_json = "{\"name\":\"new\"}";
    auto hit = tgcloud::sync::detect(local, remote);
    assert(hit.has_value());
    assert(tgcloud::sync::merge(*hit).data_json.find("new") != std::string::npos);

    // --- backup envelope ---
    auto enc = tgcloud::backup::encrypt("logical-state", "bkp-pw");
    assert(tgcloud::backup::decrypt(enc, "bkp-pw") == "logical-state");

    // --- cloud_files adapter from a sync-style record ---
    tgcloud::database::MemoryStore store;
    tgcloud::compat::CloudFiles files(store);
    tgcloud::model::CloudFile cf;
    cf.file_uuid = "uuid-1";
    cf.name = "clip.mp4";
    cf.telegram_file_id = "file-0";
    files.upsert(cf);
    assert(files.by_telegram_file("file-0")->name == "clip.mp4");

    // --- streaming seek: byte in the second 4 MiB chunk ---
    auto ranges = tgcloud::streaming::plan(4ULL * 1024ULL * 1024ULL + 100, 50,
                                           4ULL * 1024ULL * 1024ULL, 2);
    assert(!ranges.empty());
    assert(ranges.front().index == 1);

    return 0;
}
