# Task 40 — test matrix

A row is **PASS** only if an automated test ran.  
`COMPATIBLE` with Android is **not** claimed unless a real `apkforensic` artifact was used.

| ID | What | Kind | Proof | Result |
|---|---|---|---|---|
| U01–U36 | Existing unit tests (tasks 03–39) | unit | `ctest` | PASS |
| I01 | 4 MiB+ file → split → assign 5-bot pool → reconstruct after shuffle → SHA-256 | integration | `pipeline_test` | PASS |
| I02 | Gap chunk indexes rejected | integration | `pipeline_test` | PASS |
| I03 | Missing `.part` file rejected | integration | `pipeline_test` | PASS |
| C01 | Synthetic 2-chunk `.link` fixture parses | compat (synthetic) | `compat_matrix_test` + `fixtures/synthetic-share.json` | PASS |
| C02 | Generated 252-chunk manifest round-trips (index + 5 uploaders) | compat (synthetic) | `compat_matrix_test` | PASS |
| C03 | `.link` generate → import (password) | compat (synthetic) | `compat_matrix_test` | PASS |
| C04 | Sync node encrypt/decrypt + `prevId` chain + one INSERT | compat (synthetic) | `compat_matrix_test` | PASS |
| C05 | Conflict merge keeps later payload | integration | `compat_matrix_test` | PASS |
| C06 | BKP1 encrypt/decrypt | compat (synthetic) | `compat_matrix_test` | PASS |
| C07 | Sync-style `cloud_files` upsert / Telegram lookup | integration | `compat_matrix_test` | PASS |
| C08 | Byte offset maps to chunk 1 | integration | `compat_matrix_test` | PASS |
| L01 | Live `getMe`/`getChat` | live | `live_telegram_test` if `TG_BOT_1`+`TG_CHAT` | optional |
| A01 | Real Android `.link` decrypt | compat (artifact) | `apkforensic` not vendored | **NOT RUN** |
| A02 | Real sync-node decrypt | compat (artifact) | same | **NOT RUN** |
| A03 | Real BKP1 backup decrypt | compat (artifact) | same | **NOT RUN** |
| A04 | Real 252-chunk DB record parse | compat (artifact) | same | **NOT RUN** |
| F01 | Encrypted store persists chunks | unit/fix | `encrypted_store_test` | PASS |
| F02 | Sync gzip compress + raw fallback decrypt | unit/fix | `sync_crypto_test` + `full_backtest` | PASS |
| F03 | Whole-project live 5-bot + upload + .link + backup | live | `full_backtest` with env | PASS |
| F04 | Desktop HTTP shell upload + redacted diagnostics | live | local :8787 | PASS |

## Open compatibility gaps (do not paper over)

- Sync plaintext GZIP (report CONFIRMED; code encrypts raw JSON in a `SYNC1` envelope).
- Exact Android `.link` JSON key spelling / credential fields.
- Exact Telegram caption / `[CHUNK]` wire text.
- Byte-level SQLCipher Android database.

## How to run

```sh
cmake -S . -B /tmp/masavault-build -DCMAKE_BUILD_TYPE=Release
cmake --build /tmp/masavault-build --parallel 1
ctest --test-dir /tmp/masavault-build --output-on-failure
# optional live:
# TG_BOT_1=… TG_CHAT=… ctest --test-dir /tmp/masavault-build -R live_telegram_test
```

`compat_matrix_test` must be started with the repo root as cwd so it finds `fixtures/`.
