cmake_minimum_required(VERSION 3.11)

project(VisitorGameEngine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------------------------
# Backend selection (native builds)
#
# The engine renders/inputs through AbstractRenderer / AbstractInputWrapper.
# Exactly one backend is compiled per build; flip it with -DBACKEND=...
#   cmake -B build -DBACKEND=SDL2     (default native backend)
#   cmake -B build -DBACKEND=SFML     (native SFML 3)
#
# WASM builds (configured with emcmake) always use the SDL2 backend and build
# one embeddable .html per game instead of the stdin menu.
# ---------------------------------------------------------------------------
set(BACKEND "SDL2" CACHE STRING "Rendering/input backend: SDL2 or SFML")
set_property(CACHE BACKEND PROPERTY STRINGS SDL2 SFML)

# Backend-agnostic engine + games. No entry point here; none include a backend
# header, so this list is shared by every target.
set(ENGINE_SOURCES
        classes/Sprite.cpp
        classes/Visitor.cpp
        classes/GameEngine.cpp
        classes/SimpleScene.cpp
        classes/LayeredScene.cpp
        classes/AbstractRenderer.cpp
        classes/AbstractCollisionVisitor.cpp
        classes/SimpleDrawingVisitor.cpp
        classes/GridDrawingVisitor.cpp
        classes/ForceVisitor.cpp
        classes/GravityVisitor.cpp
        classes/BounceBoundsVisitor.cpp
        classes/WrapBoundsVisitor.cpp
        classes/BoundingBoxCollisionVisitor.cpp
        classes/RayCastCollisionVisitor.cpp
        classes/games/Game.cpp
        classes/games/Pong.cpp
        classes/games/Quoridor.cpp
        classes/games/TicTacToe.cpp
        )

set(SDL_BACKEND_SOURCES
        classes/SDLBackend.cpp
        classes/SDLRenderer.cpp
        classes/SDLInputWrapper.cpp
        classes/SDLTextureFactory.cpp
        )

set(SFML_BACKEND_SOURCES
        classes/SFMLBackend.cpp
        classes/SFMLRenderer.cpp
        classes/SFMLInputWrapper.cpp
        classes/TextureFactory.cpp
        )

if(EMSCRIPTEN)
    # -----------------------------------------------------------------------
    # WebAssembly: one embeddable page per game, SDL2 backend, ASYNCIFY so the
    # games' blocking loops can yield to the browser (see classes/Yield.h).
    # -----------------------------------------------------------------------
    set(EMS_OPTIONS "-sUSE_SDL=2" "-sUSE_SDL_IMAGE=2" "-sSDL2_IMAGE_FORMATS=[png]")
    set(EMS_LINK_OPTIONS ${EMS_OPTIONS}
            "-sASYNCIFY"
            "-sALLOW_MEMORY_GROWTH=1"
            "--preload-file=${CMAKE_SOURCE_DIR}/assets@/assets"
            "--shell-file=${CMAKE_SOURCE_DIR}/web/shell.html")

    # game name -> compile define
    set(GAMES Pong TicTacToe Quoridor)
    foreach(game ${GAMES})
        string(TOLOWER ${game} game_lower)
        string(TOUPPER ${game} game_upper)
        add_executable(${game_lower} main_web.cpp ${ENGINE_SOURCES} ${SDL_BACKEND_SOURCES})
        target_compile_definitions(${game_lower} PRIVATE GAME_${game_upper})
        target_compile_options(${game_lower} PRIVATE ${EMS_OPTIONS})
        target_link_options(${game_lower} PRIVATE ${EMS_LINK_OPTIONS})
        set_target_properties(${game_lower} PROPERTIES SUFFIX ".html")
    endforeach()

    message(STATUS "VisitorGameEngine WASM targets: pong, tictactoe, quoridor")

else()
    # -----------------------------------------------------------------------
    # Native: single executable with the stdin game menu.
    # -----------------------------------------------------------------------
    add_executable(VisitorGameEngine main_native.cpp ${ENGINE_SOURCES})

    if(BACKEND STREQUAL "SDL2")
        target_sources(VisitorGameEngine PRIVATE ${SDL_BACKEND_SOURCES})
        find_package(SDL2 REQUIRED)
        find_package(SDL2_image REQUIRED)
        target_link_libraries(VisitorGameEngine PRIVATE SDL2::SDL2 SDL2_image::SDL2_image)

    elseif(BACKEND STREQUAL "SFML")
        target_sources(VisitorGameEngine PRIVATE ${SFML_BACKEND_SOURCES})
        find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)
        target_link_libraries(VisitorGameEngine PRIVATE SFML::Graphics SFML::Window SFML::System)

    else()
        message(FATAL_ERROR "Unknown BACKEND '${BACKEND}'. Use SDL2 or SFML.")
    endif()

    message(STATUS "VisitorGameEngine native backend: ${BACKEND}")
endif()
