#include "Quoridor.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

// Quoridor: a 9x9 race where each player also has walls to slow the other down.
//   * Move : arrow keys step the active player's pawn one cell (desktop), or
//            tap an orthogonally-adjacent cell (touch).
//   * Wall : click / tap a grid line (or long-press on touch) drops a two-cell
//            wall there; orientation follows the line you aimed at.
//   * A wall may never completely seal a player off -- a breadth-first search
//            rejects any placement that would leave someone with no path home.
//   * Win  : reach the far row. Player 1 starts at the top, Player 2 the bottom.
//
// Note the engine never learns what a "pawn" or a "wall" is: both are plain
// Sprites, drawn by the same GridDrawingVisitor. All the rules live here.
void Quoridor::start() {
    const int CELLS = 9;            // 9x9 board
    const int SLOTS = CELLS - 1;    // interior wall-slot indices per axis (0..7)
    const int SCREEN = 540;         // 540 / 9 = 60px cells
    const int block = SCREEN / CELLS;
    const int START_WALLS = 10;

    const std::string wallTex = "./assets/img/bbox.png";    // black wall
    const std::string p1Tex = "./assets/img/blueX.png";     // player 1 pawn
    const std::string p2Tex = "./assets/img/redSquare.png"; // player 2 pawn

    //open a window via the build-time backend (SDL2, SFML, ...)
    std::shared_ptr<Backend> backend = createDefaultBackend();
    Platform platform = backend->create(SCREEN, SCREEN, "Quoridor");
    std::shared_ptr<AbstractRenderer> sr = platform.renderer;
    std::shared_ptr<AbstractInputWrapper> in = platform.input;

    std::shared_ptr<GameEngine> ge = std::make_shared<GameEngine>();
    std::shared_ptr<GridDrawingVisitor> draw = std::make_shared<GridDrawingVisitor>(CELLS, CELLS, sr);

    // pawn LOGICAL positions (col,row); the sprites are just the visual mirror,
    // inset slightly inside their cell so the grid lines stay readable.
    int p1c = CELLS / 2, p1r = 0;          // top, races to the bottom row
    int p2c = CELLS / 2, p2r = CELLS - 1;  // bottom, races to the top row
    const double inset = 0.1, pawnSize = 0.8;
    auto p1 = std::make_shared<Sprite>(p1Tex, p1c + inset, p1r + inset, pawnSize, pawnSize);
    auto p2 = std::make_shared<Sprite>(p2Tex, p2c + inset, p2r + inset, pawnSize, pawnSize);
    ge->addSprite(p1);
    ge->addSprite(p2);
    ge->addVisitor(draw); // drawing visitor goes last

    // Wall state. hWall[r][c] = horizontal wall on the line below row r spanning
    // columns c..c+1; vWall[r][c] = vertical wall right of col c spanning rows
    // r..r+1. Valid slot indices are 0..SLOTS-1.
    bool hWall[CELLS][CELLS] = {{false}};
    bool vWall[CELLS][CELLS] = {{false}};
    std::vector<std::shared_ptr<Sprite>> wallSprites; // scene keeps them alive too
    int wallsLeft[3] = {0, START_WALLS, START_WALLS}; // indexed by player number

    auto inBounds = [&](int r, int c) { return r >= 0 && r < CELLS && c >= 0 && c < CELLS; };

    // Is an orthogonal step from (r,c) to (nr,nc) blocked by a wall?
    auto blocked = [&](int r, int c, int nr, int nc) -> bool {
        if (nr == r - 1 && nc == c) { int l = r - 1; return (c < SLOTS && hWall[l][c]) || (c - 1 >= 0 && hWall[l][c - 1]); }
        if (nr == r + 1 && nc == c) { int l = r;     return (c < SLOTS && hWall[l][c]) || (c - 1 >= 0 && hWall[l][c - 1]); }
        if (nc == c - 1 && nr == r) { int l = c - 1; return (r < SLOTS && vWall[r][l]) || (r - 1 >= 0 && vWall[r - 1][l]); }
        if (nc == c + 1 && nr == r) { int l = c;     return (r < SLOTS && vWall[r][l]) || (r - 1 >= 0 && vWall[r - 1][l]); }
        return true; // not an orthogonal neighbour
    };

    // BFS: can a pawn at (sr,sc) still reach goalRow given the current walls?
    auto reachable = [&](int sr_, int sc_, int goalRow) -> bool {
        bool seen[CELLS][CELLS] = {{false}};
        std::queue<std::pair<int, int>> q;
        q.push({sr_, sc_});
        seen[sr_][sc_] = true;
        const int dr[4] = {-1, 1, 0, 0}, dc[4] = {0, 0, -1, 1};
        while (!q.empty()) {
            std::pair<int, int> cur = q.front();
            q.pop();
            if (cur.first == goalRow) return true;
            for (int k = 0; k < 4; ++k) {
                int nr = cur.first + dr[k], nc = cur.second + dc[k];
                if (inBounds(nr, nc) && !seen[nr][nc] && !blocked(cur.first, cur.second, nr, nc)) {
                    seen[nr][nc] = true;
                    q.push({nr, nc});
                }
            }
        }
        return false;
    };

    bool playing = true;
    int turn = 1;
    auto switchTurn = [&]() { turn = (turn == 1) ? 2 : 1; };
    auto announceTurn = [&]() {
        std::cout << "Player " << turn << "'s turn (walls left: " << wallsLeft[turn] << ")" << std::endl;
    };

    // Attempt a pawn move (with a straight jump over the opponent) for the
    // active player; advances the turn and checks for a win on success.
    auto tryMove = [&](int dr, int dc) {
        int &cr = (turn == 1) ? p1r : p2r;
        int &cc = (turn == 1) ? p1c : p2c;
        int opr = (turn == 1) ? p2r : p1r;
        int opc = (turn == 1) ? p2c : p1c;
        int nr = cr + dr, nc = cc + dc;
        if (!inBounds(nr, nc) || blocked(cr, cc, nr, nc)) return;
        if (nr == opr && nc == opc) {
            // jump straight over the opponent if the landing cell is open
            int jr = nr + dr, jc = nc + dc;
            if (!inBounds(jr, jc) || blocked(nr, nc, jr, jc)) return;
            nr = jr;
            nc = jc;
        }
        cr = nr;
        cc = nc;
        auto &pawn = (turn == 1) ? p1 : p2;
        pawn->setXY(cc + inset, cr + inset);
        if (turn == 1 && p1r == CELLS - 1) { std::cout << "Player 1 reaches the far side -- Player 1 wins!" << std::endl; playing = false; return; }
        if (turn == 2 && p2r == 0)         { std::cout << "Player 2 reaches the far side -- Player 2 wins!" << std::endl; playing = false; return; }
        switchTurn();
        announceTurn();
    };

    // Attempt to place a wall for the active player. Rejects overlaps, crossings,
    // an empty wall stash, and -- via BFS -- any placement that traps a player.
    auto tryWall = [&](int r, int c, bool horizontal) {
        if (r < 0 || r >= SLOTS || c < 0 || c >= SLOTS) return;
        if (wallsLeft[turn] <= 0) { std::cout << "No walls left!" << std::endl; return; }
        if (horizontal) {
            if (hWall[r][c]) return;
            if (c - 1 >= 0 && hWall[r][c - 1]) return;
            if (c + 1 < SLOTS && hWall[r][c + 1]) return;
            if (vWall[r][c]) return; // would cross a vertical wall
            hWall[r][c] = true;
            if (!(reachable(p1r, p1c, CELLS - 1) && reachable(p2r, p2c, 0))) {
                hWall[r][c] = false;
                std::cout << "That wall would trap a player." << std::endl;
                return;
            }
            auto w = std::make_shared<Sprite>(wallTex, c, (r + 1) - 0.075, 2, 0.15);
            wallSprites.push_back(w);
            ge->addSprite(w);
        } else {
            if (vWall[r][c]) return;
            if (r - 1 >= 0 && vWall[r - 1][c]) return;
            if (r + 1 < SLOTS && vWall[r + 1][c]) return;
            if (hWall[r][c]) return; // would cross a horizontal wall
            vWall[r][c] = true;
            if (!(reachable(p1r, p1c, CELLS - 1) && reachable(p2r, p2c, 0))) {
                vWall[r][c] = false;
                std::cout << "That wall would trap a player." << std::endl;
                return;
            }
            auto w = std::make_shared<Sprite>(wallTex, (c + 1) - 0.075, r, 0.15, 2);
            wallSprites.push_back(w);
            ge->addSprite(w);
        }
        wallsLeft[turn]--;
        switchTurn();
        announceTurn();
    };

    // Translate a click/tap into either a wall placement or a pawn move.
    auto handleClick = [&](int px, int py, bool longPress) {
        double fx = px / (double) block; // board coordinates, in cells
        double fy = py / (double) block;
        int cellC = (int) fx, cellR = (int) fy;
        double inX = fx - cellC, inY = fy - cellR;
        double distV = std::min(inX, 1.0 - inX); // distance to nearest vertical line
        double distH = std::min(inY, 1.0 - inY); // distance to nearest horizontal line
        bool nearLine = std::min(distH, distV) < 0.25;

        if (longPress || nearLine) {
            // wall: orientation follows whichever grid line you aimed closest to
            bool horizontal = distH < distV;
            if (horizontal) {
                int line = (int) (fy + 0.5); // nearest horizontal grid line index
                tryWall(line - 1, (int) fx, true);
            } else {
                int line = (int) (fx + 0.5);
                tryWall((int) fy, line - 1, false);
            }
        } else {
            // interior tap: move the active pawn if this cell is adjacent
            int cr = (turn == 1) ? p1r : p2r;
            int cc = (turn == 1) ? p1c : p2c;
            int dr = cellR - cr, dc = cellC - cc;
            if (std::abs(dr) + std::abs(dc) == 1) tryMove(dr, dc);
        }
    };

    std::cout << "Quoridor: Player 1 (blue) starts at the top and races to the bottom; Player 2 (red) does the reverse." << std::endl;
    std::cout << "Move: arrow keys or tap an adjacent cell. Wall: click/tap a grid line (long-press on touch). 10 walls each." << std::endl;
    announceTurn();

    Clock tick;
    bool prevDir[4] = {false, false, false, false}; // Up, Down, Left, Right edges
    bool wasDown = false, longHandled = false, sawLong = false;

    while (draw->isOpen() && playing) {
        frameYield(16); // browser: yield to the event loop; native: no-op
        if (tick.getElapsedMilliseconds() > 30) {
            tick.restart();

            // keyboard movement, edge-detected so a held key steps exactly once
            std::vector<Key> keys = in->getKeyPresses();
            bool dir[4] = {false, false, false, false};
            for (Key &k : keys) {
                if (k == Key::Up) dir[0] = true;
                else if (k == Key::Down) dir[1] = true;
                else if (k == Key::Left) dir[2] = true;
                else if (k == Key::Right) dir[3] = true;
            }
            if (playing && dir[0] && !prevDir[0]) tryMove(-1, 0);
            if (playing && dir[1] && !prevDir[1]) tryMove(1, 0);
            if (playing && dir[2] && !prevDir[2]) tryMove(0, -1);
            if (playing && dir[3] && !prevDir[3]) tryMove(0, 1);
            for (int i = 0; i < 4; ++i) prevDir[i] = dir[i];

            // mouse / touch: long-press places a wall immediately; a short tap is
            // resolved on release (wall near a line, otherwise a move).
            click c = in->getLastClick();
            bool down = c.isLeft != 0;
            if (down && !wasDown) { longHandled = false; sawLong = false; }
            if (playing && down && c.isLong && !longHandled) {
                handleClick(c.x, c.y, true);
                longHandled = true;
                sawLong = true;
            }
            if (playing && !down && wasDown && !sawLong) {
                handleClick(c.x, c.y, false);
            }
            wasDown = down;
        }
        ge->update();
        draw->draw();
    }
    std::cout << "QUORIDOR CLOSED" << std::endl;
}
