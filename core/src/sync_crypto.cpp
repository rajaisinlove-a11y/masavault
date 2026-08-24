#include "tgcloud/sync/crypto.hpp"

#include <cstring>
#include <random>
#include <stdexcept>
#include <zlib.h>
#include <openssl/evp.h>

namespace tgcloud::sync {
namespace {

std::string gzip_compress(std::string_view in) {
    z_stream zs{};
    if (deflateInit2(&zs, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 16 + MAX_WBITS, 8, Z_DEFAULT_STRATEGY) != Z_OK)
        throw std::runtime_error("gzip init failed");
    zs.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(in.data()));
    zs.avail_in = static_cast<uInt>(in.size());
    std::string out;
    char buf[4096];
    int ret;
    do {
        zs.next_out = reinterpret_cast<Bytef*>(buf);
        zs.avail_out = sizeof buf;
        ret = deflate(&zs, Z_FINISH);
        out.append(buf, sizeof buf - zs.avail_out);
    } while (ret == Z_OK);
    deflateEnd(&zs);
    if (ret != Z_STREAM_END) throw std::runtime_error("gzip compress failed");
    return out;
}

bool gzip_decompress(std::string_view in, std::string& out) {
    if (in.size() < 2 || static_cast<unsigned char>(in[0]) != 0x1f || static_cast<unsigned char>(in[1]) != 0x8b)
        return false;
    z_stream zs{};
    if (inflateInit2(&zs, 16 + MAX_WBITS) != Z_OK) return false;
    zs.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(in.data()));
    zs.avail_in = static_cast<uInt>(in.size());
    char buf[4096];
    int ret;
    do {
        zs.next_out = reinterpret_cast<Bytef*>(buf);
        zs.avail_out = sizeof buf;
        ret = inflate(&zs, Z_NO_FLUSH);
        out.append(buf, sizeof buf - zs.avail_out);
        if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            inflateEnd(&zs);
            return false;
        }
    } while (ret != Z_STREAM_END);
    inflateEnd(&zs);
    return true;
}

}  // namespace

std::string encrypt(std::string_view in, std::string_view pw) {
    auto gz = gzip_compress(in);
    unsigned char salt[16], iv[12], key[32];
    std::random_device r;
    for (auto& x : salt) x = static_cast<unsigned char>(r());
    for (auto& x : iv) x = static_cast<unsigned char>(r());
    PKCS5_PBKDF2_HMAC(pw.data(), static_cast<int>(pw.size()), salt, 16, 100000, EVP_sha256(), 32, key);
    EVP_CIPHER_CTX* c = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(c, EVP_aes_256_gcm(), nullptr, key, iv);
    std::string e(gz.size() + 16, 0);
    int n = 0, z = 0;
    EVP_EncryptUpdate(c, reinterpret_cast<unsigned char*>(e.data()), &n,
                      reinterpret_cast<const unsigned char*>(gz.data()), static_cast<int>(gz.size()));
    EVP_EncryptFinal_ex(c, reinterpret_cast<unsigned char*>(e.data()) + n, &z);
    unsigned char tag[16];
    EVP_CIPHER_CTX_ctrl(c, EVP_CTRL_GCM_GET_TAG, 16, tag);
    EVP_CIPHER_CTX_free(c);
    std::string o = "SYNC1";
    o.append(reinterpret_cast<char*>(salt), 16);
    o.append(reinterpret_cast<char*>(iv), 12);
    o.append(e.data(), n + z);
    o.append(reinterpret_cast<char*>(tag), 16);
    return o;
}

std::string decrypt(std::string_view b, std::string_view pw) {
    if (b.size() < 49 || b.substr(0, 5) != "SYNC1") throw std::runtime_error("invalid sync envelope");
    unsigned char key[32];
    PKCS5_PBKDF2_HMAC(pw.data(), static_cast<int>(pw.size()),
                      reinterpret_cast<const unsigned char*>(b.data()) + 5, 16, 100000, EVP_sha256(), 32, key);
    EVP_CIPHER_CTX* c = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(c, EVP_aes_256_gcm(), nullptr, key, reinterpret_cast<const unsigned char*>(b.data()) + 21);
    int n = 0, z = 0;
    std::string p(b.size() - 49, 0);
    EVP_DecryptUpdate(c, reinterpret_cast<unsigned char*>(p.data()), &n,
                      reinterpret_cast<const unsigned char*>(b.data()) + 33, static_cast<int>(b.size() - 49));
    EVP_CIPHER_CTX_ctrl(c, EVP_CTRL_GCM_SET_TAG, 16, const_cast<char*>(b.data() + b.size() - 16));
    if (EVP_DecryptFinal_ex(c, reinterpret_cast<unsigned char*>(p.data()) + n, &z) <= 0) {
        EVP_CIPHER_CTX_free(c);
        throw std::runtime_error("sync authentication failed");
    }
    EVP_CIPHER_CTX_free(c);
    p.resize(static_cast<size_t>(n + z));
    std::string unzipped;
    if (gzip_decompress(p, unzipped)) return unzipped;
    return p;  // older SYNC1 payloads without gzip still decrypt
}

}  // namespace tgcloud::sync
