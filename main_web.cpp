// Web entry point. The browser has no stdin menu, so each WASM module runs one
// game directly, chosen at compile time with -DGAME_<NAME>. CMake builds one
// target per game (pong.html, tictactoe.html, quoridor.html).
#include "classes/games/Pong.h"
#include "classes/games/Quoridor.h"
#include "classes/games/TicTacToe.h"
#include <memory>

int main() {
    std::shared_ptr<Game> game;
#if defined(GAME_TICTACTOE)
    game = std::make_shared<TicTacToe>();
#elif defined(GAME_QUORIDOR)
    game = std::make_shared<Quoridor>();
#else // default: Pong
    game = std::make_shared<Pong>();
#endif
    game->start();
    return 0;
}
