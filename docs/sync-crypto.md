# Sync Cryptography

The sync crypto boundary implements the recovered parameters: PBKDF2-HMAC-SHA256, 100,000 iterations, 32-byte key, random 16-byte salt, random 12-byte GCM IV, AES-256-GCM, and 16-byte authentication tag. Envelope prefix is `SYNC1`; exact Android compression/field encoding remains fixture-gated.
