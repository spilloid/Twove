#!/usr/bin/env bash
# Build the WebAssembly target: one playable .html/.js/.wasm/.data bundle per
# game, using the SDL2 backend through Emscripten.
#
# Requires the Emscripten SDK on PATH (emcmake, emcc). See:
#   https://emscripten.org/docs/getting_started/downloads.html
#
#   Usage: ./scripts/build-web.sh [--package]
#     --package  also copy the built games into docs/demos/ for GitHub Pages
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$ROOT/build-web"

command -v emcmake >/dev/null 2>&1 || {
  echo "error: emcmake not found. Activate the Emscripten SDK first:" >&2
  echo "       source /path/to/emsdk/emsdk_env.sh" >&2
  exit 1
}

echo "Configuring (Emscripten / SDL2 / WASM)..."
emcmake cmake -B "$BUILD_DIR" -S "$ROOT" -DCMAKE_BUILD_TYPE=Release

echo "Building pong, tictactoe, quoridor..."
cmake --build "$BUILD_DIR" -j4

echo
echo "Built modules in $BUILD_DIR :"
ls -1 "$BUILD_DIR"/*.html 2>/dev/null || true

if [[ "${1:-}" == "--package" ]]; then
  echo
  echo "Packaging into docs/demos/ ..."
  mkdir -p "$ROOT/docs/demos"
  for game in pong tictactoe quoridor; do
    for ext in html js wasm data; do
      [[ -f "$BUILD_DIR/$game.$ext" ]] && cp "$BUILD_DIR/$game.$ext" "$ROOT/docs/demos/"
    done
  done
  echo "Copied. Commit docs/demos/ to publish via GitHub Pages."
fi
