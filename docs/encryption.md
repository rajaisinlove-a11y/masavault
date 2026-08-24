# Local Encryption

The local store uses a versioned `TGDB1` envelope: PBKDF2-HMAC-SHA256 with 100,000 iterations derives a 32-byte key from a random 16-byte salt; AES-256-GCM uses a random 12-byte IV and authenticates the serialized logical-file records. This is encrypted desktop persistence, not a claim of byte-level SQLCipher/Android database compatibility.
