# Packaging & Distribution

The twove ships as **one codebase, two delivery targets**:

| Target | Output | Entry point | Backend | Build tool |
|---|---|---|---|---|
| **Native desktop** | a single executable with a stdin game menu | `main_native.cpp` | SDL2 (default) or SFML | CMake |
| **WebAssembly** | one playable `.html` bundle per game | `main_web.cpp` | SDL2 → WASM | Emscripten + CMake |

The engine and game code are identical for both. The only thing that changes is
the backend wired in behind `AbstractRenderer` / `AbstractInputWrapper` and which
entry point is compiled — that's the whole point of the `Backend` seam.

---

## 1. Native desktop executable

### Prerequisites

- **CMake** ≥ 3.11 and a **C++17** compiler.
- **SDL2** and **SDL2_image** discoverable by CMake.

| Platform | Get the dependencies |
|---|---|
| Windows | [vcpkg](https://vcpkg.io): `vcpkg install sdl2 sdl2-image` |
| macOS | Homebrew: `brew install sdl2 sdl2_image cmake` |
| Linux (Debian/Ubuntu) | `sudo apt install cmake libsdl2-dev libsdl2-image-dev` |

### Build

The repo ships wrapper scripts under [`scripts/`](../scripts):

```powershell
# Windows (PowerShell) — point at your vcpkg toolchain so CMake finds SDL2
./scripts/build-native.ps1 -Toolchain C:\vcpkg\scripts\buildsystems\vcpkg.cmake
```

```bash
# macOS / Linux
./scripts/build-native.sh
```

Or invoke CMake directly:

```bash
cmake -B build -DBACKEND=SDL2          # default backend
cmake --build build -j4
```

`-DBACKEND=SFML` selects the SFML 3 backend instead. Only the selected backend's
translation units are compiled and linked.

### Run

```bash
./build/twove          # MSVC: build/Release/twove.exe
```

A menu reads from stdin: `0` Tic-Tac-Toe, `1` Pong, `2` Quoridor, `3` quit.

### Distributing the .exe (Windows)

A CMake/SDL2 build links SDL2 dynamically, so the executable needs its DLLs and
the `assets/` folder beside it. A minimal distributable folder:

```
twove/
  twove.exe
  SDL2.dll
  SDL2_image.dll
  assets/            ← copied from the repo root
```

Copy the DLLs from your vcpkg `installed/x64-windows/bin` directory (or build
with `-DBUILD_SHARED_LIBS=OFF` against static SDL2 triplets to fold them into the
exe). Zip the folder and ship it — no installer required.

> The game loads textures from a relative `assets/` path, so the working
> directory must contain `assets/` at launch. Keep them together.

---

## 2. WebAssembly bundle

The same C++ compiles to WASM with no source changes. Each game becomes a
self-contained web page; the SDL2 backend is provided by Emscripten's
`-sUSE_SDL=2`, and [`classes/Yield.h`](../classes/Yield.h) + `-sASYNCIFY` let the
games' blocking loops cooperate with the browser event loop.

### Prerequisites

- The [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html),
  activated so `emcmake`/`emcc` are on `PATH`:

  ```bash
  source /path/to/emsdk/emsdk_env.sh
  ```

### Build

```bash
./scripts/build-web.sh             # outputs to build-web/
```

Or directly:

```bash
emcmake cmake -B build-web -DCMAKE_BUILD_TYPE=Release
cmake --build build-web -j4
```

This produces, per game, four files in `build-web/`:

| File | Role |
|---|---|
| `pong.html` | shell page ([`web/shell.html`](../web/shell.html)) with the canvas |
| `pong.js` | Emscripten loader / glue |
| `pong.wasm` | the compiled engine + game |
| `pong.data` | the preloaded `assets/` virtual filesystem |

All four are needed to run a game; serve them together (a `file://` open will be
blocked by browser CORS — use a local server: `python -m http.server`).

### Build flags (set in [`CMakeLists.txt`](../CMakeLists.txt))

- `-sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS=[png]` — SDL2 + PNG loading.
- `-sASYNCIFY` — lets `Game::start()`'s loop yield to the browser each frame.
- `-sALLOW_MEMORY_GROWTH=1` — grow the heap on demand.
- `--preload-file assets@/assets` — bakes textures/fonts into `pong.data`.
- `--shell-file web/shell.html` — the HTML wrapper around the canvas.

### Publishing to GitHub Pages

The live site is served from [`docs/`](.). To refresh the playable demos:

```bash
./scripts/build-web.sh --package   # builds, then copies into docs/demos/
git add docs/demos && git commit -m "Refresh WASM demos"
git push
```

`docs/index.html` embeds each `docs/demos/<game>.html` in an `<iframe>`. GitHub
Pages serves `docs/` on push; `docs/.nojekyll` keeps Pages from mangling the
Emscripten output.

---

## 3. Docker

Two Dockerfiles let you build and serve without a local toolchain — useful for
CI or for compile-checking a change on a machine with no compiler.

### Serve the WASM demos ([`Dockerfile`](../Dockerfile))

Multi-stage: stage 1 compiles every game to WASM with Emscripten (so a build
break fails `docker build`), stage 2 serves the bundles from `nginx:alpine`.

```bash
docker build -t vge-web .
docker run --rm -p 8080:80 vge-web      # → http://localhost:8080
```

The nginx config ([`docker/nginx.conf`](../docker/nginx.conf)) is deliberately
minimal; its one non-default touch is forcing `.wasm` to be served as
`application/wasm` so the browser can stream-compile it. The landing page
([`docker/index.html`](../docker/index.html)) just links the three games.

### Compile-check the native build ([`Dockerfile.native`](../Dockerfile.native))

The games are GUI apps, so this verifies the desktop target *builds* on a clean
Linux box with SDL2 — it doesn't run a window.

```bash
docker build -f Dockerfile.native -t vge-native .
docker run --rm vge-native                          # prints the built binary
docker build -f Dockerfile.native --build-arg BACKEND=SFML -t vge-sfml .
```

Both builds honour [`.dockerignore`](../.dockerignore), which trims the context
to just the source, assets, CMake, entry points, and web shell.

---

## Which target produces what

```
                         main_native.cpp ─┐
                                          ├─► CMake + SDL2 ─► twove(.exe)   [desktop]
   classes/  (engine + games, identical) ─┤
                                          └─► emcmake + SDL2 ─► pong.{html,js,wasm,data} [browser]
                         main_web.cpp ─────┘
```

One engine. Pick the entry point and the toolchain; the game code never moves.
