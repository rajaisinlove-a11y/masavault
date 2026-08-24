# Backup Compatibility

The backup crypto boundary follows the recovered BKP1 format: 16-byte salt, 16-byte IV, AES-256-CBC with PKCS#7 padding, and key SHA-256 of UTF-8 password concatenated with salt. It encrypts logical backup payloads; physical Telegram objects are not included.
