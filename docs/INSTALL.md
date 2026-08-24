# Install

## Dependencies (Debian / Ubuntu)

```sh
sudo apt-get install -y cmake g++ libssl-dev libcurl4-openssl-dev
```

## Build, test, install

```sh
cmake -S . -B /tmp/masavault-build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build /tmp/masavault-build --parallel 1
ctest --test-dir /tmp/masavault-build --output-on-failure
cmake --install /tmp/masavault-build
masavault-desktop
```

The binary prints `TG Cloud Desktop` and exits 0. That is expected in v0.1.0.

## Release tarball

```sh
cmake -S . -B /tmp/masavault-build -DCMAKE_BUILD_TYPE=Release
cmake --build /tmp/masavault-build --parallel 1 --target package
# produces /tmp/masavault-build/masavault-0.1.0-Linux.tar.gz
```

Or: `scripts/package.sh`

## Optional live Telegram test

```sh
TG_BOT_1='…' TG_CHAT='-100…' \
  /tmp/masavault-build/tests/live_telegram_test
```

Never put those values in Git.
