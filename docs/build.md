# Linux Build Baseline

The project currently uses a dependency-light C++20 core and CMake build. The desktop executable is intentionally a bootstrap shell; application functionality begins in later tasks. UI toolkit selection remains an explicit architecture decision before the UI phase.

## Requirements

- CMake 3.16+
- C++20 compiler

## Configure, build, test

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
./build/desktop/masavault-desktop
```

`build/` is generated and must not be committed or retained when workspace pressure matters.

Packaging and prefix install are documented in `docs/INSTALL.md`. The architecture handoff is `docs/FINAL_ARCHITECTURE.md`.
