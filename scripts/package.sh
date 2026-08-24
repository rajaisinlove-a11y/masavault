#!/bin/sh
# Build a Release binary package in /tmp. Does not keep build trees in the repo.
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BUILD=${MASAVAULT_BUILD_DIR:-/tmp/masavault-build}
cmake -S "$ROOT" -B "$BUILD" -DCMAKE_BUILD_TYPE=Release
cmake --build "$BUILD" --parallel 1
ctest --test-dir "$BUILD" --output-on-failure
cmake --build "$BUILD" --target package
echo "package: $BUILD/masavault-0.1.0-Linux.tar.gz"
