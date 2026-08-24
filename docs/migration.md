# Android/Desktop Migration

The migration boundary preserves the recovered logical `cloud_files` identity and Telegram metadata fields without copying private APK/database artifacts. It is deliberately a field-preserving interchange representation; byte-level SQLCipher database compatibility remains unproven until a real Android fixture is available.
