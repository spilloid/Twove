<div align="center">

# Visitor Game Engine

**A C++17 game engine where behavior is composed, not inherited.**
Every entity is a `Sprite`. Every behavior is a `Visitor`. The engine loop is two
virtual calls per sprite per frame — and that double dispatch *is* the engine.

[**▶ Play the demos**](https://spilloid.github.io/Visitor-Game-Engine/) · [Packaging guide](docs/PACKAGING.md) · [API docs](https://spilloid.github.io/Visitor-Game-Engine/html/index.html)

`Visitor` · `Abstract Factory` · `Strategy` · SDL2 · WebAssembly · zero `dynamic_cast`

</div>

---

## Why it exists

Most engines model behavior with inheritance — `Enemy extends Character`,
`FlyingEnemy extends Enemy` — until the hierarchy collapses under its own weight.
This engine takes the opposite stance:

- **Sprites are pure data.** Position, velocity, size, a texture path. Nothing else.
- **Visitors are pure behavior.** Each implements one operation across every sprite.
- **Adding a feature means adding a class** — never editing `Sprite`, `Scene`, or `GameEngine`.

Want gravity? Add a `GravityVisitor`. Collision? `BoundingBoxCollisionVisitor`.
Compose them at startup, in order. The engine is open for extension and closed
for modification, in the most literal sense.

## One codebase, two targets

The same engine and game code runs as a **native desktop app** *or* in the
**browser via WebAssembly** — the backend slots in behind `AbstractRenderer` /
`AbstractInputWrapper`, chosen at build time. No game code changes.

| Target | Output | Backend | Build |
|---|---|---|---|
| 🖥️ Desktop | `VisitorGameEngine(.exe)` — stdin game menu | SDL2 (default) / SFML | CMake |
| 🌐 Web | one playable `.html` per game | SDL2 → WASM | Emscripten + CMake |

▶ **[Try the WebAssembly demos in your browser →](https://spilloid.github.io/Visitor-Game-Engine/)**

## Quick start (desktop)

Requires CMake, a C++17 compiler, and SDL2 + SDL2_image
([how to get them per-platform](docs/PACKAGING.md#1-native-desktop-executable)).

```bash
# Windows (PowerShell) — pass your vcpkg toolchain so CMake finds SDL2
./scripts/build-native.ps1 -Toolchain C:\vcpkg\scripts\buildsystems\vcpkg.cmake

# macOS / Linux
./scripts/build-native.sh

# …or CMake directly
cmake -B build -DBACKEND=SDL2 && cmake --build build -j4
./build/VisitorGameEngine        # menu: 0 TicTacToe · 1 Pong · 2 Quoridor · 3 quit
```

For the browser build, GitHub Pages packaging, and how to distribute the `.exe`,
see **[docs/PACKAGING.md](docs/PACKAGING.md)**.

## How double dispatch works

The core loop in `GameEngine.cpp` is five lines:

```cpp
void GameEngine::update() {
    for (auto &sceneVisitor : this->sceneVisitors)
        this->scene->accept(sceneVisitor.get());   // dispatch #1: which scene?
}
```

`scene->accept()` is a virtual call — the vtable resolves `SimpleScene`,
`LayeredScene`, or anything else you write. Inside the scene:

```cpp
void SimpleScene::accept(Visitor* v) {
    for (auto &sprite : this->spriteList)
        v->visit(sprite.get());                     // dispatch #2: which visitor?
}
```

`v->visit()` is the second virtual call; each concrete Visitor has its own. No
`dynamic_cast`, no `typeid`, no if-else chains on type — just pointer indirection
the CPU branch predictor eats for breakfast.

Dispatch hands the sprite/visitor across as a **raw, non-owning pointer** — the
scene owns the graph, the visitor only observes it — so the hot loop carries no
`shared_ptr` refcount traffic. The double dispatch stays as light as the two
vtable lookups it's meant to be.

```
GameEngine::update()
  └─ for each Visitor:
        scene->accept(visitor)          ← vtable[scene]
          └─ for each Sprite:
                visitor->visit(sprite)  ← vtable[visitor]
```

## Visitor catalog

| Visitor | Category | What it does |
|---|---|---|
| `SimpleDrawingVisitor` | Rendering | Draws each sprite at its `(x, y)` position |
| `GridDrawingVisitor` | Rendering | Converts grid coordinates to pixels, then draws |
| `ForceVisitor` | Physics | Moves each sprite by its `(dx, dy)` each frame |
| `GravityVisitor` | Physics | Adds a constant acceleration to `dy` |
| `BounceBoundsVisitor` | Physics | Reflects `dx`/`dy` at the scene boundary |
| `WrapBoundsVisitor` | Physics | Teleports a sprite from one edge to the opposite |
| `BoundingBoxCollisionVisitor` | Collision | AABB test against a single watched sprite |
| `RayCastCollisionVisitor` | Collision | Casts a ray from a moving sprite along its velocity; reports everything in its path (ray-vs-AABB). Pong's AI uses it to know when the ball is incoming |

## Architecture

```
GameEngine
  ├── scene ──► AbstractScene ──┬── SimpleScene   (flat sprite list)
  │                             └── LayeredScene  (z-ordered layers)
  └── visitors ──► list<Visitor> (composed at startup, run in order)

AbstractRenderer      ──► SDLRenderer      │  selected by the Backend
AbstractInputWrapper  ──► SDLInputWrapper  │  abstract factory at build time
        (SFML and Emscripten/WASM are alternate backends behind the same seams)
```

The backend lives entirely behind those interfaces, produced by the `Backend`
abstract factory. Game code never names a concrete backend or touches a window
handle — which is exactly what lets the same code target desktop and the web.

## Games

| Game | Status | Notes |
|---|---|---|
| **Pong** | Playable | Two-player; player 2 is a raycast AI that only chases the incoming ball (so it's beatable); scoring + round resets |
| **Quoridor** | Playable | Real 9×9 board with **walls**; arrow-keys/tap to move, click a grid line to wall (long-press on touch); BFS forbids trapping a player; race to the far row |
| **TicTacToe** | Playable | Click to place; turn tracking, win/draw detection, X/O textures on a visible 3×3 grid |

## Project layout

```
main_native.cpp        ← desktop entry point (stdin game menu)
main_web.cpp           ← WebAssembly entry point (one game per module)
CMakeLists.txt         ← build for both targets; -DBACKEND switches native backend
scripts/               ← build-native.{ps1,sh}, build-web.sh
classes/
  Sprite · Visitor · GameEngine            ← the untouchable core
  AbstractScene / SimpleScene / LayeredScene
  *Visitor.{h,cpp}                         ← all behavior lives here
  Backend.h + SDL*/SFML*                   ← swappable backends behind the seams
  games/  Pong · Quoridor · TicTacToe
web/shell.html         ← HTML wrapper for the WASM canvas
assets/                ← textures + font (preloaded into the .data file for WASM)
docs/                  ← GitHub Pages site, playable demos, Doxygen API reference
```

## Design principles

- **Gang-of-Four patterns over cleverness** — Visitor, Abstract Factory, Strategy.
- **SOLID, especially OCP + DIP** — new behavior is new code; high-level code
  depends on abstractions, never concretes.
- **Seams are sacred** — the backend (SDL2 / SFML / WASM) is fully isolated.
- **RAII everywhere** — `shared_ptr` for graph nodes, `unique_ptr` with custom
  deleters for C handles; no naked `new`/`delete`.

See [CLAUDE.md](CLAUDE.md) for the full house style.

## Known limitations

- **`GravityVisitor` reads backwards** — subtracts from `dy` (up in screen space); correct, counter-intuitive.
- **`LayeredScene`** has no bounds check on the layer index; an out-of-range layer silently no-ops.
- **Text rendering** — not implemented; scores and turns print to the console. `AbstractRenderer` draws sprites and primitives (lines/rects), not text.
- **Click events** — the input wrapper polls the current mouse/touch state rather than queuing events, so games rising-edge-detect clicks themselves.
- **SFML backend** — predates the SDL2 work; needs an SFML 2→3 port before `-DBACKEND=SFML` compiles.

---

<div align="center">
Built for CS400 Game Development · <a href="docs/PACKAGING.md">Packaging</a> · <a href="https://spilloid.github.io/Visitor-Game-Engine/">Live demos</a>
</div>
