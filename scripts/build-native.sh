#!/usr/bin/env bash
# Configure and build the native desktop executable (macOS / Linux).
# Produces build/twove — the stdin game-select menu.
#
# Requires CMake, a C++17 compiler, and SDL2 + SDL2_image (Homebrew / apt).
#   Usage: ./scripts/build-native.sh [SDL2|SFML]
set -euo pipefail

BACKEND="${1:-SDL2}"
ROOT="$(cd "$(dirname "$0")/.." && pwd)"

echo "Configuring ($BACKEND)..."
cmake -B "$ROOT/build" -S "$ROOT" -DBACKEND="$BACKEND"

echo "Building..."
cmake --build "$ROOT/build" -j4

echo
echo "Done. Run the game menu with:  $ROOT/build/twove"
