// Task 40 — local end-to-end pipeline (no Telegram).
//
// Create a file slightly larger than 4 MiB, split it, assign bots,
// shuffle the chunk list, reconstruct by logical index, check SHA-256.
// Then prove missing / gap chunks are rejected.

#include <cassert>
#include <filesystem>
#include <fstream>
#include <openssl/sha.h>
#include <sstream>
#include <stdexcept>
#include <string>

#include "tgcloud/chunking/chunker.hpp"
#include "tgcloud/scheduler/bot_pool.hpp"
#include "tgcloud/transfer/assignment.hpp"
#include "tgcloud/transfer/reconstruct.hpp"

namespace fs = std::filesystem;

static std::string sha256_file(const fs::path& p) {
    std::ifstream f(p, std::ios::binary);
    SHA256_CTX c;
    SHA256_Init(&c);
    char buf[65536];
    while (f.read(buf, sizeof buf) || f.gcount()) SHA256_Update(&c, buf, f.gcount());
    unsigned char d[SHA256_DIGEST_LENGTH];
    SHA256_Final(d, &c);
    std::ostringstream o;
    for (auto x : d) o << std::hex << (x >> 4) << (x & 0xf);
    return o.str();
}

int main() {
    auto root = fs::temp_directory_path() / "masavault-pipeline";
    fs::remove_all(root);
    fs::create_directories(root);
    auto src = root / "in.bin";
    {
        std::ofstream o(src, std::ios::binary);
        const std::size_t n = tgcloud::chunking::compatible_chunk_size + 12345;
        std::string block(4096, '\0');
        unsigned char v = 1;
        std::size_t left = n;
        while (left) {
            for (char& c : block) c = static_cast<char>(v++);
            auto w = std::min(left, block.size());
            o.write(block.data(), static_cast<std::streamsize>(w));
            left -= w;
        }
    }
    const auto want = sha256_file(src);
    auto parts = root / "parts";
    std::vector<tgcloud::model::ChunkRecord> recs;
    tgcloud::chunking::split(src, parts, [&](const tgcloud::chunking::Chunk& ch, const fs::path&) {
        tgcloud::model::ChunkRecord r;
        r.index = ch.index;
        r.total = ch.total;
        r.size = ch.size;
        r.sha256 = ch.sha256;
        recs.push_back(r);
    });
    assert(recs.size() == 2);

    tgcloud::scheduler::BotPool pool({{"b1"}, {"b2"}, {"b3"}, {"b4"}, {"b5"}});
    auto assigned = tgcloud::transfer::assign_chunks(recs, pool);
    assert(assigned.size() == 2);
    assert(recs[0].uploader_alias == "b1");
    assert(recs[1].uploader_alias == "b2");

    // Message order is not chunk order: reverse the vector on purpose.
    std::swap(recs[0], recs[1]);
    auto out = root / "out.bin";
    tgcloud::transfer::reconstruct(recs, parts, out);
    assert(sha256_file(out) == want);

    // Gap: indexes 0 and 2, no index 1.
    std::vector<tgcloud::model::ChunkRecord> gap = recs;
    gap[0].index = 0;
    gap[1].index = 2;
    bool gap_rejected = false;
    try {
        tgcloud::transfer::reconstruct(gap, parts, root / "gap.bin");
    } catch (const std::runtime_error&) {
        gap_rejected = true;
    }
    assert(gap_rejected);

    fs::remove(parts / "1.part");
    bool missing_rejected = false;
    // restore sequential indexes so the missing *file* is what fails
    recs[0].index = 1;
    recs[1].index = 0;
    try {
        tgcloud::transfer::reconstruct(recs, parts, root / "miss.bin");
    } catch (const std::runtime_error&) {
        missing_rejected = true;
    }
    assert(missing_rejected);

    fs::remove_all(root);
    return 0;
}
