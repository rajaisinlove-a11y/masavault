// Task 06 — TGDB1 encrypted file store: write, reload, reject bad password.
// Also persist per-chunk Telegram ids (compatibility fix).

#include <cassert>
#include <filesystem>

#include "tgcloud/database/encrypted_store.hpp"

int main() {
    auto p = std::filesystem::temp_directory_path() / "tgcloud-test.db";
    std::filesystem::remove(p);
    {
        tgcloud::database::EncryptedFileStore s(p, "pw");
        tgcloud::model::CloudFile f;
        f.name = "secret";
        f.size_bytes = 42;
        f.file_uuid = "uuid-1";
        f.chunks.push_back({0, 2, 20, "h0", 11, "fid0", "uid0", "bot-a"});
        f.chunks.push_back({1, 2, 22, "h1", 12, "fid1", "uid1", "bot-b"});
        s.put_file(f);
    }
    {
        tgcloud::database::EncryptedFileStore s(p, "pw");
        auto got = s.get_file(1);
        assert(got);
        assert(got->name == "secret");
        assert(got->chunks.size() == 2);
        assert(got->chunks[1].telegram_file_id == "fid1");
        assert(got->chunks[0].uploader_alias == "bot-a");
    }
    bool bad = false;
    try {
        tgcloud::database::EncryptedFileStore x(p, "bad");
    } catch (...) {
        bad = true;
    }
    assert(bad);
    std::filesystem::remove(p);
    return 0;
}
