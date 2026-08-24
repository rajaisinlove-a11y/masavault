# `.link` Encryption

The link crypto boundary follows the recovered parameters: PBKDF2-HMAC-SHA256 with 10,000 iterations, 32-byte key, 16-byte salt, 16-byte IV, AES-256-CBC, and OpenSSL PKCS#7-compatible padding. Binary layout is salt + IV + ciphertext. Exact Android plaintext encoding remains fixture-gated.
