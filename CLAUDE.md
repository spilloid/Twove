# CLAUDE.md

Guidance for working in the twove. This file captures how this
codebase is built and — more importantly — the C++ style and design instincts it
is written with. Match them; don't fight them.

## What this project is

A C++ game engine where **behavior is a Visitor and every entity is a `Sprite`**.
No entity subclasses, no `switch` on type, no `dynamic_cast`. The engine loop is
two virtual calls per sprite per frame — `scene->accept(visitor)` then
`visitor->visit(sprite)`. That double dispatch *is* the engine. Adding a feature
means adding a class, never editing `Sprite`, `Scene`, or `GameEngine`.

If a change tempts you to add a type check or a god-object, stop — there is
almost always a visitor- or pattern-shaped answer that keeps the core untouched.

## Design principles (the non-negotiables)

- **Gang-of-Four patterns over cleverness.** Prefer a named, recognizable
  pattern (Visitor, Abstract Factory, Strategy, Factory Method) to a clever
  one-off. Readability and extensibility beat brevity.
- **SOLID, especially OCP + DIP.** New behavior is new code, not modified code.
  High-level code depends on abstractions (`AbstractRenderer`,
  `AbstractInputWrapper`, `Backend`, `Visitor`), never on concretes.
- **Seams are sacred.** The backend (SDL2 / SFML / WASM) lives entirely behind
  `AbstractRenderer` / `AbstractInputWrapper`, produced by the `Backend` abstract
  factory. Game code must never name a concrete backend or touch a window handle.
- **Separation of concerns.** One responsibility per class. Sprites are pure
  data; visitors are pure behavior; renderers only render; factories only build.
- **Proper algorithms, no magic numbers.** Use named constants and enums
  (e.g. the backend-neutral `Key` enum) rather than raw scancodes or literals.

## C++ conventions (inferred house style)

- **C++17.** RAII everywhere; no naked `new`/`delete`.
- **Smart pointers by default.** `std::shared_ptr` for shared graph nodes
  (sprites, visitors, scenes, the renderer/input pair). `std::unique_ptr` with a
  **custom deleter** for owned C handles (see `SDLRAII.h`). A **raw pointer means
  "non-owning observer"** and nothing else — never an ownership transfer.
- **Ownership is expressed in the type, and destruction order in member order.**
  Don't write a destructor you can get for free from RAII. When order matters
  (e.g. textures before renderer before window), encode it by declaring members
  in the right order and let reverse-order destruction do the work.
- **One class per `.h`/`.cpp` pair.** `#ifndef`/`#define` include guards.
- **Doxygen `@brief` on every class and public method.** Keep them; they are
  part of the style. Comments may carry personality — that's welcome — but they
  must still be accurate.
- **Naming:** `PascalCase` types, `camelCase` methods and variables,
  `ALL_CAPS` for local constants in game setup.
- **`this->` prefix** for member access (used consistently across the codebase).
- **`= default`** for trivial constructors/destructors. Give every polymorphic
  base a `virtual ~T() = default`.
- **Abstract base classes are interfaces:** pure virtual methods, named
  `Abstract*` or a bare role name (`Visitor`, `Backend`). Concrete classes are
  named for their backend/behavior (`SDLRenderer`, `GravityVisitor`).
- Watch for `*/` inside block comments when writing pointer types (`Foo* / Bar*`
  closes the comment). It will bite you.

## Build

CMake with a build-time backend switch — the single seam that picks the target:

```bash
cmake -B build -DBACKEND=SDL2      # default; SDL2 is installed and is the WASM path too
cmake --build build -j4
./build/twove          # menu: 0 TicTacToe, 1 Pong, 2 Quoridor, 3 quit
```

`-DBACKEND=SFML` selects the SFML target instead (see status below). Only the
selected backend's translation units are compiled and linked; `createDefaultBackend()`
is defined by exactly one backend `.cpp` per build.

### Backends
- **SDL2** (`classes/SDL*.{h,cpp}`): primary, working on macOS. Uses SDL2 +
  SDL2_image (Homebrew). The same code is the Emscripten/WASM path
  (`-sUSE_SDL=2 -sUSE_SDL_IMAGE=2`).
- **SFML** (`classes/SFML*.{h,cpp}`, `TextureFactory.*`): original backend, was
  written for SFML 2; Homebrew now ships SFML 3 (breaking API changes). Needs a
  v2→v3 port before the `-DBACKEND=SFML` target compiles.

## Environment caveat (this machine)

The Command Line Tools install is missing its toolchain libc++ headers
(`/Library/Developer/CommandLineTools/usr/include/c++/v1`), so a bare
`#include <iostream>` fails. Permanent fix: `sudo xcode-select --install` (or
reinstall CLT). Local-only workaround used for building tonight — **do not commit
this into CMakeLists.txt**:

```bash
cmake -B build -DBACKEND=SDL2 \
  -DCMAKE_CXX_FLAGS="-isystem /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1"
```

## Verifying changes

These are GUI apps that loop until the window closes and read the menu from
stdin. To smoke-test headlessly: `printf '1\n' | ./build/twove &`,
let it run ~1.5s, check the log for the game's startup line / gameplay output,
then `pkill -f twove`. Buffered stdout is lost on kill, so rely on
`std::endl`-flushed lines printed during the run.
