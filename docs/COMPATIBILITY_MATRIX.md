# TG Cloud Compatibility Matrix

Status values: **CONFIRMED** = directly supported by forensic evidence; **OBSERVED** = runtime behavior reported/tested in evidence; **INFERRED** = architectural interpretation requiring fixtures; **UNKNOWN** = not established.

| Area | Compatibility fact | Status | Planned proof |
|---|---|---:|---|
| Identity | Android package `com.telegram.cloud`, version 1.2.0 | CONFIRMED | Reference report |
| Logical files | `cloud_files` separates logical metadata from Telegram objects | CONFIRMED | SQL schema/inventory |
| Physical storage | Telegram Bot API documents/messages/chunks | CONFIRMED | Classes, native symbols, report |
| Chunking | 4 MiB compatible chunk constant; final short chunk | CONFIRMED | Reference report; fixture test |
| Chunk order | Logical chunk index is required; message order must not be assumed | INFERRED | 252-chunk artifact parser |
| Bot pool | Multiple uploader tokens assigned across chunks; five-bot artifact exists | OBSERVED | Sanitized manifest fixture |
| Upload recovery | Persisted resume fields/methods exist, but failed upload is cancelled in observed UI | OBSERVED | Explicit compatibility behavior test |
| Local state | `cloud_files`, upload/download tasks, gallery, sync logs/metadata | CONFIRMED | SQL strings |
| Sync crypto | PBKDF2-HMAC-SHA256, 100k, 32-byte key, 16-byte salt, 12-byte GCM IV, AES-256-GCM, gzip JSON | CONFIRMED | Report/artifact validation |
| Sync nodes | `entries`, `prevId`, timestamp; node may contain one operation | OBSERVED | Real sync-node evidence |
| `.link` crypto | PBKDF2-HMAC-SHA256, 10k, AES-256-CBC, 16-byte salt/IV, padded ciphertext | CONFIRMED | Real manifest analysis |
| `.link` payload | JSON manifest includes file/chunk Telegram IDs, hashes, uploader data | OBSERVED | 305-file artifact analysis |
| Backup | `telegram_cloud.db.enc`, `.env.enc`, manifest; remote objects excluded | OBSERVED | Backup report |
| Backup crypto | BKP1, SHA-256(password UTF-8 concatenated with salt), AES-256-CBC | CONFIRMED | Report/artifact validation |
| Migration | Android exposes desktop migration methods | CONFIRMED | APK method inventory |
| Streaming | Chunk-aware offset/seek classes exist | CONFIRMED | APK classes; runtime validation pending |
| Native layer | ARM JNI library handles transfer/database/backup operations | CONFIRMED | ELF/JNI inventory |
| Exact wire metadata | Captions, field ordering, API conventions | UNKNOWN | Trace artifacts and integration fixtures |

## Data-model mapping

| Production concept | Compatibility fields | Source status |
|---|---|---|
| `CloudFile` | UUID/local ID, name, MIME, size, timestamps, checksum, Telegram message/file/unique IDs, share data, uploader assignments | CONFIRMED schema fields; UUID semantics UNKNOWN |
| `ChunkRecord` | logical index, total, size, hash, Telegram message/file IDs, unique ID, uploader identity | Manifest/report evidence; exact metadata encoding UNKNOWN |
| `TransferTask` | URI/path, display name, size, status, progress, error, completed chunks, temp directory, token offset | CONFIRMED schema fields |
| `SyncLog` | log ID, timestamp, device ID, operation, table, primary key, data, previous data, remote message ID, checksum | CONFIRMED schema fields |
| `SyncNode` | node ID, `prevId`, timestamp, entries/operations | OBSERVED/INFERRED |
| `GalleryMedia` | local path, filename, MIME, dimensions, duration, thumbnail, sync state, remote IDs | CONFIRMED schema fields |
| `Backup` | encrypted DB/config plus manifest; no physical Telegram files | OBSERVED |
| `ShareManifest` | version/type, file metadata, chunk list, Telegram IDs, hashes, uploader data | OBSERVED |

## Compatibility policy

Adapters preserve original names/encodings at boundaries. Internal names may be idiomatic, but no field is dropped without a documented migration rule. A row becomes `COMPATIBLE` only after a fixture or integration test passes; until then it remains `EVIDENCE_ONLY`, `INFERRED`, or `UNKNOWN`. Real credentials and private artifacts never enter production tests.
