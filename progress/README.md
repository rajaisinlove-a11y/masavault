# Progress map

Read this first. Then open the numbered `task-NN.txt` for details.

Each finished task has:

1. an implementation commit (`task-NN: …`)
2. a checkpoint commit (`checkpoint: task-NN`)
3. a test binary (see table)

Status: **01–39 complete** (UI tests 35–38 were missing from Git and restored). **40 is next.** Live Telegram recheck: `backtest-task-01-38.txt`.

| Task | Title | Test to run | What that test proves |
|---:|---|---|---|
| 01 | Architecture index | *(docs only)* | Evidence labels, two-repo split |
| 02 | Compatibility matrix | *(docs only)* | Field map, CONFIRMED/UNKNOWN |
| 03 | CMake / C++20 project | `core_smoke_test` | Library links, product name |
| 04 | Domain models | `domain_test` | CloudFile, chunks, transfer, sync log |
| 05 | Local store | `store_test` | CRUD + pagination |
| 06 | Encrypted store TGDB1 | `encrypted_store_test` | Reload + wrong password fails |
| 07 | Multi-bot pool | `bot_pool_test` | Acquire / disable / stats |
| 08–09 | Bot API + validation | `validator_test` | Token/channel result classes |
| 10 | Direct upload | *(live BotClient)* | sendDocument exists on client |
| 11 | 4 MiB chunker | `chunker_test` | Split, hash, short last chunk |
| 12 | Manifest | `manifest_test` | File → chunk JSON mapping |
| 13 | Per-chunk assignment | `assignment_test` | Uploader alias on each chunk |
| 14 | Transfer queue | `queue_test` | Bounded workers |
| 15 | Download | `downloader_test` | getFile / download_to boundary |
| 16 | Reconstruct | `reconstruct_test` | Index order, reject gaps |
| 17 | Cancel / errors | `control_test` | Token + error taxonomy |
| 18 | Progress / ETA | `progress_test` | Byte snapshot |
| 19 | Retry / backoff | `policy_test` | Retry-After, give up |
| 20 | cloud_files adapter | `cloud_files_test` | UUID upsert, Telegram lookup |
| 21 | Sync log store | `log_store_test` | Append-only pending logs |
| 22 | Sync crypto | `sync_crypto_test` | PBKDF2 100k + AES-GCM |
| 23 | Sync node | `node_test` | id / prevId / entries |
| 24 | Sync chain | `chain_test` | Oldest-first, break/cycle |
| 25 | Conflicts | `conflict_test` | Timestamp merge |
| 26 | Devices | `device_test` | Device id + chain head |
| 27 | `.link` crypto | `link_crypto_test` | PBKDF2 10k + AES-CBC |
| 28 | `.link` generate | `link_generator_test` | Single + batch files |
| 29 | `.link` import | `link_import_test` | Decrypt to JSON |
| 30 | Backup BKP1 | `backup_test` | Encrypt / decrypt / bad password |
| 31 | Android migration | `migrator_test` | Field-preserving round trip |
| 32 | Gallery catalog | `media_test` | Paginated media rows |
| 33 | Streaming ranges | `range_test` | Offset → chunk range |
| 34 | File browser URI | `browser_test` | `tgcloud://` entries |
| 35 | Transfer center model | `transfer_center_test` | Rows: task + progress + bots |
| 36 | Sidebar / dashboard | `navigation_test` | Pages + navigate |
| 37 | Setup wizard model | `setup_test` | Storage vs sync, aliases only |
| 38 | Share / backup / gallery UI | `actions_test` | Batch share, backup export |
| 39 | Settings / security / diagnostics | `settings_test` | Redact secrets, export log |
| 40 | End-to-end / compatibility matrix | — | Not started |
| 41 | Package + handoff | — | Not started |

## How to continue

1. Highest complete `progress/task-NN.txt` wins.
2. Do **not** restart finished tasks.
3. Next incomplete number is the only active task.
4. If something is broken, write `progress/task-NN-failure.txt` and stop.

Reference forensics (read-only): `https://github.com/rajaisinlove-a11y/apkforensic`
