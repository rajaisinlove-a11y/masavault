# TG Cloud Desktop — Architecture Baseline

Task 01 repository archaeology baseline. The forensic source is `apkforensic`; this repository contains production source only.

## Evidence discipline

- **CONFIRMED:** recovered reports/inventories identify package `com.telegram.cloud`, version 1.2.0; Kotlin/Android app; SQLCipher/SQLite schema; Telegram Bot API transport; native JNI layer; 4 MiB chunk constant; sync-node and `.link` crypto parameters documented in the reference reports.
- **OBSERVED:** failed uploads cancel and require manual re-selection; large chunked videos were previewed/streamed; real artifacts include a 252-chunk mapping, encrypted sync node, and 305-file `.link` manifest.
- **INFERRED:** logical cloud metadata is separate from physical Telegram messages; chunk index, not message order, is authoritative; sync nodes form a `prevId` chain.
- **UNKNOWN:** exact wire metadata/caption conventions, every migration detail, complete streaming request semantics, and any undocumented Android edge cases. These require fixtures/source tracing before compatibility claims.

## Subsystem map

1. **Domain/database:** `cloud_files`, `upload_tasks`, `download_tasks`, `gallery_media`, `sync_logs`, `sync_metadata`; logical file and chunk metadata are first-class.
2. **Telegram:** Bot API document send/getFile/download, channel/message identifiers, multiple bot identities, retry/rate-limit handling.
3. **Chunking:** compatible 4 MiB chunks, final short chunk, manifest mapping, per-chunk hashes and uploader assignments.
4. **Scheduling:** bounded concurrent queue, dynamic bot assignment, per-bot health/rate limits and statistics.
5. **Sync:** append-only logs → compressed/encrypted sync nodes → Telegram → decrypt/replay/conflict handling; preserve `prevId` chain.
6. **Crypto:** sync PBKDF2-HMAC-SHA256/100k + AES-256-GCM; `.link` PBKDF2-HMAC-SHA256/10k + AES-256-CBC; backup BKP1 SHA-256(password||salt) + AES-256-CBC. Parameters remain fixture-gated.
7. **Sharing/backup:** encrypted portable manifests and encrypted logical-state/config backup; physical remote objects are not contained in backup.
8. **Gallery/streaming:** separate media metadata/cache; chunk-aware offset reads and buffering, not whole-file RAM loads.
9. **Native/platform:** original has ARM JNI transfer/database/backup layer. Desktop core must be UI-independent; Linux secret storage and file integration belong at platform boundary.
10. **UI/CLI:** modern Linux cloud browser, transfer center, setup/bot/sync/security screens; no transport logic in widgets.

## Compatibility boundaries

Production may use clean internal models, but adapters must preserve Telegram IDs, logical chunk indexes, uploader assignments, sync fields, and encryption encodings. No real credentials or private artifacts enter this repository. Compatibility fixtures are synthetic or minimal sanitized derivatives; real evidence remains in `apkforensic`.

## Implementation order

1. Establish compatibility matrix and fixture policy.
2. Select/init Linux stack and build/test harness.
3. Domain models and encrypted local database abstraction.
4. Telegram client/auth, chunking, manifest, scheduler, transfer queue.
5. Sync crypto/nodes/chain/conflict/device sync.
6. `.link`, backup, migration compatibility.
7. downloads/reconstruction, gallery and streaming.
8. desktop shell, browser, transfer center, setup/settings/security.
9. integration/compatibility matrix, packaging and final handoff.

## Repository policy

`apkforensic` is immutable reference material by default. `masavault` is the production desktop project. Keep generated builds/caches out of Git and delete them after verification. Every task ends with implementation/docs/tests, a dedicated commit, a small checkpoint, push, and workspace-size verification.
