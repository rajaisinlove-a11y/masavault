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

Run the desktop shell:

```sh
/tmp/masavault-build/desktop/masavault-desktop --port 8787 --www desktop/www
```

Open `http://127.0.0.1:8787`. `--name` still prints `TG Cloud Desktop` and exits.

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
