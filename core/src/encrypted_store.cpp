#include "tgcloud/database/encrypted_store.hpp"

#include <cstring>
#include <fstream>
#include <openssl/evp.h>
#include <random>
#include <stdexcept>

namespace tgcloud::database {
namespace {

void put_str(std::string& b, const std::string& s) {
    uint32_t n = static_cast<uint32_t>(s.size());
    b.append(reinterpret_cast<char*>(&n), 4);
    b += s;
}

std::string get_str(const std::string& b, size_t& p) {
    if (p + 4 > b.size()) throw std::runtime_error("corrupt store");
    uint32_t n;
    memcpy(&n, b.data() + p, 4);
    p += 4;
    if (p + n > b.size()) throw std::runtime_error("corrupt store");
    auto s = b.substr(p, n);
    p += n;
    return s;
}

template <class T>
void put_pod(std::string& b, T v) {
    b.append(reinterpret_cast<char*>(&v), sizeof v);
}

template <class T>
T get_pod(const std::string& b, size_t& p) {
    if (p + sizeof(T) > b.size()) throw std::runtime_error("corrupt store");
    T v{};
    memcpy(&v, b.data() + p, sizeof v);
    p += sizeof(T);
    return v;
}

}  // namespace

EncryptedFileStore::EncryptedFileStore(std::filesystem::path p, std::string pw)
    : path_(std::move(p)), password_(std::move(pw)) {
    load();
}

void EncryptedFileStore::load() {
    if (!std::filesystem::exists(path_)) return;
    std::ifstream in(path_, std::ios::binary);
    std::string b((std::istreambuf_iterator<char>(in)), {});
    if (b.size() < 49 || b.substr(0, 5) != "TGDB1") throw std::runtime_error("invalid encrypted store");
    unsigned char key[32], iv[12];
    if (!PKCS5_PBKDF2_HMAC(password_.c_str(), static_cast<int>(password_.size()),
                           reinterpret_cast<unsigned char*>(b.data()) + 5, 16, 100000, EVP_sha256(), 32, key))
        throw std::runtime_error("key derivation failed");
    memcpy(iv, b.data() + 21, 12);
    int n = static_cast<int>(b.size() - 49);
    EVP_CIPHER_CTX* c = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(c, EVP_aes_256_gcm(), nullptr, key, iv);
    std::string plain(static_cast<size_t>(n) + 16, 0);
    int out = 0, fin = 0;
    EVP_DecryptUpdate(c, reinterpret_cast<unsigned char*>(plain.data()), &out,
                      reinterpret_cast<unsigned char*>(b.data()) + 33, n);
    EVP_CIPHER_CTX_ctrl(c, EVP_CTRL_GCM_SET_TAG, 16, b.data() + b.size() - 16);
    if (EVP_DecryptFinal_ex(c, reinterpret_cast<unsigned char*>(plain.data()) + out, &fin) <= 0) {
        EVP_CIPHER_CTX_free(c);
        throw std::runtime_error("authentication failed");
    }
    EVP_CIPHER_CTX_free(c);
    plain.resize(static_cast<size_t>(out + fin));
    size_t p = 0;
    auto count = get_pod<uint32_t>(plain, p);
    for (uint32_t i = 0; i < count; i++) {
        model::CloudFile f;
        f.local_id = get_pod<model::Id>(plain, p);
        f.file_uuid = get_str(plain, p);
        f.name = get_str(plain, p);
        f.mime_type = get_str(plain, p);
        f.size_bytes = get_pod<std::uint64_t>(plain, p);
        f.uploaded_at = get_pod<std::int64_t>(plain, p);
        f.checksum = get_str(plain, p);
        f.telegram_message_id = get_pod<model::Id>(plain, p);
        f.telegram_file_id = get_str(plain, p);
        f.telegram_file_unique_id = get_str(plain, p);
        if (p + 4 <= plain.size()) {
            auto nchunks = get_pod<uint32_t>(plain, p);
            f.chunks.reserve(nchunks);
            for (uint32_t k = 0; k < nchunks; k++) {
                model::ChunkRecord ch;
                ch.index = get_pod<std::uint32_t>(plain, p);
                ch.total = get_pod<std::uint32_t>(plain, p);
                ch.size = get_pod<std::uint64_t>(plain, p);
                ch.sha256 = get_str(plain, p);
                ch.telegram_message_id = get_pod<model::Id>(plain, p);
                ch.telegram_file_id = get_str(plain, p);
                ch.telegram_file_unique_id = get_str(plain, p);
                ch.uploader_alias = get_str(plain, p);
                f.chunks.push_back(std::move(ch));
            }
        }
        memory_.put_file(std::move(f));
    }
}

void EncryptedFileStore::flush() const {
    std::string plain;
    auto all = memory_.list_files(static_cast<size_t>(-1), 0);
    put_pod<uint32_t>(plain, static_cast<uint32_t>(all.size()));
    for (auto& f : all) {
        put_pod(plain, f.local_id);
        put_str(plain, f.file_uuid);
        put_str(plain, f.name);
        put_str(plain, f.mime_type);
        put_pod(plain, f.size_bytes);
        put_pod(plain, f.uploaded_at);
        put_str(plain, f.checksum);
        put_pod(plain, f.telegram_message_id);
        put_str(plain, f.telegram_file_id);
        put_str(plain, f.telegram_file_unique_id);
        put_pod<uint32_t>(plain, static_cast<uint32_t>(f.chunks.size()));
        for (auto& ch : f.chunks) {
            put_pod(plain, ch.index);
            put_pod(plain, ch.total);
            put_pod(plain, ch.size);
            put_str(plain, ch.sha256);
            put_pod(plain, ch.telegram_message_id);
            put_str(plain, ch.telegram_file_id);
            put_str(plain, ch.telegram_file_unique_id);
            put_str(plain, ch.uploader_alias);
        }
    }
    unsigned char salt[16], iv[12];
    std::random_device r;
    for (auto& x : salt) x = static_cast<unsigned char>(r());
    for (auto& x : iv) x = static_cast<unsigned char>(r());
    unsigned char key[32];
    PKCS5_PBKDF2_HMAC(password_.c_str(), static_cast<int>(password_.size()), salt, 16, 100000, EVP_sha256(), 32, key);
    EVP_CIPHER_CTX* c = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(c, EVP_aes_256_gcm(), nullptr, key, iv);
    std::string enc(plain.size() + 16, 0);
    int out = 0, fin = 0;
    EVP_EncryptUpdate(c, reinterpret_cast<unsigned char*>(enc.data()), &out,
                      reinterpret_cast<unsigned char*>(plain.data()), static_cast<int>(plain.size()));
    EVP_EncryptFinal_ex(c, reinterpret_cast<unsigned char*>(enc.data()) + out, &fin);
    unsigned char tag[16];
    EVP_CIPHER_CTX_ctrl(c, EVP_CTRL_GCM_GET_TAG, 16, tag);
    EVP_CIPHER_CTX_free(c);
    std::ofstream o(path_, std::ios::binary | std::ios::trunc);
    o.write("TGDB1", 5);
    o.write(reinterpret_cast<char*>(salt), 16);
    o.write(reinterpret_cast<char*>(iv), 12);
    o.write(enc.data(), out + fin);
    o.write(reinterpret_cast<char*>(tag), 16);
}

}  // namespace tgcloud::database
