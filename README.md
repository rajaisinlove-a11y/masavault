# MasaVault — TG Cloud Desktop

Linux desktop client for the existing **TG Cloud** Android ecosystem (`com.telegram.cloud` 1.2.0).

Telegram Bot API is the storage backend. This is **not** a generic Telegram Drive clone. It must stay compatible with existing channels, bots, 4 MiB chunks, sync nodes, `.link` shares, and BKP1 backups.

## How to read this repository

| If you want… | Open… |
|---|---|
| What is built so far | [`progress/README.md`](progress/README.md) |
| What each numbered task did | `progress/task-01.txt` … `progress/task-41.txt` |
| What a piece of code is supposed to do | the matching test in `tests/unit/` |
| Architecture / evidence labels | [`docs/architecture.md`](docs/architecture.md) |
| Compatibility claims | [`docs/COMPATIBILITY_MATRIX.md`](docs/COMPATIBILITY_MATRIX.md) |
| What is actually tested | [`docs/TEST_MATRIX.md`](docs/TEST_MATRIX.md) |

**Rule:** a feature is only “compatible” if a test passed. `CONFIRMED` / `OBSERVED` / `INFERRED` / `UNKNOWN` in the docs are not marketing words.

## Layout

```
core/        storage, Telegram, chunking, sync, crypto  (no UI)
desktop/     desktop models + bootstrap executable
tests/       one small C++ program per subsystem
docs/        short notes, one file per subsystem
progress/    handoff checkpoints another person can resume from
```

The desktop binary is still a bootstrap shell (`TG Cloud Desktop`). Tasks 34–39 are UI *models* (data the future Qt/GTK window will bind to). There is no window toolkit in the tree yet.

## Build and test

```sh
cmake -S . -B /tmp/masavault-build -DCMAKE_BUILD_TYPE=Release
cmake --build /tmp/masavault-build --parallel 1
ctest --test-dir /tmp/masavault-build --output-on-failure
```

Needs: CMake 3.16+, C++20, OpenSSL, libcurl.

Install, packaging, and a secret-free example config: [`docs/INSTALL.md`](docs/INSTALL.md).  
Handoff for the next developer: [`docs/FINAL_ARCHITECTURE.md`](docs/FINAL_ARCHITECTURE.md).  
If something breaks: [`docs/TROUBLESHOOTING.md`](docs/TROUBLESHOOTING.md).

```sh
cmake --install /tmp/masavault-build
# or
sh scripts/package.sh
```

Do not commit `build/`. Delete it after you verify.

## Secrets

Never commit bot tokens, sync passwords, or a GitHub PAT. The conversation dump that once sat at the repo root was removed for that reason.
