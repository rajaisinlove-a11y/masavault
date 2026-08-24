# Reference Data Model

The Android evidence separates logical cloud records, Telegram physical objects, transfer state, gallery state, and synchronization state. Production adapters should retain Telegram message/file IDs and logical chunk indexes rather than reconstructing order from messages. See `docs/COMPATIBILITY_MATRIX.md`; raw evidence is in `apkforensic`.
