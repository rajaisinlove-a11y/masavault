# Final architecture and handoff (Task 41)

**Product:** MasaVault / TG Cloud Desktop  
**Repos:** production `https://github.com/rajaisinlove-a11y/masavault` · forensics `https://github.com/rajaisinlove-a11y/apkforensic` (private)  
**Branch:** `main`  
**Version:** 0.1.0  

This is a **core-complete, UI-incomplete** Linux rebuild of Android TG Cloud 1.2.0. Telegram Bot API is the object store. The desktop binary is a bootstrap that prints `TG Cloud Desktop`. Widgets must not talk to Telegram.

## What exists

```
core/        C++20 library: models, TGDB1 store, curl Bot API, 4 MiB chunker,
             bot pool, queue, reconstruct, retry, sync (SYNC1/GCM), .link (CBC),
             BKP1, gallery catalog, range planner, secret redaction
desktop/     view-models (browser, transfer, nav, setup, actions, settings)
             + masavault-desktop bootstrap
tests/       unit + integration + optional live Telegram
fixtures/    synthetic share/sync JSON only
docs/        one short file per subsystem + TEST_MATRIX + this handoff
progress/    task-01.txt … task-41.txt  (handoff for the next agent)
```

## Data flow (intended)

```
local file
  → 4 MiB split (last chunk short)
  → BotPool assign per chunk
  → sendDocument (Bot API)
  → persist CloudFile + ChunkRecord (index is law, not message order)
  → getFile + download parts
  → reconstruct by index + SHA-256

local change → sync log → SYNC1 encrypt → Telegram
peer → decrypt → prevId chain → conflict merge → local DB

share → .link (PBKDF2 10k + AES-CBC)  |  backup → BKP1 (logical state only)
```

## Crypto (compatibility mode)

| Use | KDF | Cipher | Notes |
|---|---|---|---|
| Sync | PBKDF2-HMAC-SHA256, 100k | AES-256-GCM, 16-byte salt, 12-byte IV | Envelope `SYNC1`. Report also wants GZIP JSON **before** encrypt — **not implemented** |
| `.link` | PBKDF2-HMAC-SHA256, 10k | AES-256-CBC, 16+16 salt/IV | `salt\|\|iv\|\|ciphertext` |
| Backup | SHA-256(UTF-8 password \|\| salt) | AES-256-CBC | Magic `BKP1` |
| Local DB | PBKDF2 100k | AES-256-GCM | Envelope `TGDB1` — not SQLCipher |

## How another agent continues

1. Read `progress/README.md`, then the highest `progress/task-NN.txt`.
2. Do **not** restart 01–41.
3. Next product work is **not** another paper task. It is:
   - pick **Qt 6** or **GTK4/libadwaita**;
   - bind existing view-models to a real window (sidebar, browser, transfer center, setup);
   - wire `BotClient` + chunker + queue into that shell;
   - store tokens in libsecret/KWallet, never in `SettingsModel`;
   - add GZIP to sync if Android artifacts require it;
   - run real `apkforensic` `.link` / sync-node / backup fixtures **without committing them**.
4. Keep the loop: implement → test → commit `task-NN` → push → `progress/task-NN.txt` → push → delete `build/`.
5. Workspace budget ≈ 128 MB persistent. Build in `/tmp`.

## Honest non-goals of v0.1.0

The 41-task protocol is finished. The **user-facing cloud client is not**. Missing: painted UI, OS secret store, GVfs/FUSE, live multi-GB soak, Android SQLCipher byte import, exact Telegram captions.

See `docs/TEST_MATRIX.md` before saying “compatible.”
