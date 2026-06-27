#include "TicTacToe.h"

#include "../VectorDrawing.h"

namespace {
const std::string kBlank = VectorSprites::Blank; // empty cell
const std::string kX = VectorSprites::MarkX;     // player 1
const std::string kO = VectorSprites::MarkO;     // player 2

// Returns the mark (1 or 2) that occupies a full line, or 0 if none does.
int winner(const int board[3][3]) {
    const int lines[8][3][2] = {
            {{0, 0}, {0, 1}, {0, 2}}, {{1, 0}, {1, 1}, {1, 2}}, {{2, 0}, {2, 1}, {2, 2}}, // rows
            {{0, 0}, {1, 0}, {2, 0}}, {{0, 1}, {1, 1}, {2, 1}}, {{0, 2}, {1, 2}, {2, 2}}, // cols
            {{0, 0}, {1, 1}, {2, 2}}, {{0, 2}, {1, 1}, {2, 0}},                           // diagonals
    };
    for (auto &line : lines) {
        int a = board[line[0][0]][line[0][1]];
        int b = board[line[1][0]][line[1][1]];
        int c = board[line[2][0]][line[2][1]];
        if (a != 0 && a == b && b == c)
            return a;
    }
    return 0;
}
} // namespace

void TicTacToe::start() {
    const int MAXX = 500, MAXY = 500, CELLS = 3;

    //open a window via the build-time backend (SDL2, SFML, ...)
    std::shared_ptr<Backend> backend = createDefaultBackend();
    Platform platform = backend->create(MAXX, MAXY, "Tic Tac Toe");
    std::shared_ptr<AbstractRenderer> ar = platform.renderer;
    std::shared_ptr<AbstractInputWrapper> in = platform.input;

    std::shared_ptr<GameEngine> ge = std::make_shared<GameEngine>();
    std::shared_ptr<GridDrawingVisitor> draw = std::make_shared<GridDrawingVisitor>(CELLS, CELLS, ar);
    ge->addVisitor(draw);

    // One persistent sprite per cell, in grid coordinates. Empty cells draw
    // nothing; placing a mark just swaps the vector sprite at runtime.
    const double markInset = 0.14;
    const double markSize = 1.0 - markInset * 2.0;
    std::shared_ptr<Sprite> cells[3][3];
    int board[3][3] = {{0}};
    for (int row = 0; row < CELLS; row++)
        for (int col = 0; col < CELLS; col++) {
            cells[row][col] = std::make_shared<Sprite>(kBlank, col + markInset, row + markInset, markSize, markSize);
            ge->addSprite(cells[row][col]);
        }

    int current = 1;          // 1 = X, 2 = O
    bool wasPressed = false;   // for rising-edge click detection
    bool gameOver = false;
    int placed = 0;
    Clock tick;

    std::cout << "Tic Tac Toe: Player 1 is the blue X, Player 2 is the red O. Click a cell." << std::endl;

    while (draw->isOpen()) {
        frameYield(16); // browser: yield to the event loop; native: no-op
        if (tick.getElapsedMilliseconds() > 30) {
            tick.restart();
            click c = in->getLastClick();
            bool pressed = c.isLeft != 0;

            if (!gameOver && pressed && !wasPressed) {
                if (c.x >= 0 && c.x < MAXX && c.y >= 0 && c.y < MAXY) {
                    int col = c.x * CELLS / MAXX;
                    int row = c.y * CELLS / MAXY;
                    if (board[row][col] == 0) {
                        board[row][col] = current;
                        cells[row][col]->setTextureLocation(current == 1 ? kX : kO);
                        placed++;

                        int w = winner(board);
                        if (w != 0) {
                            std::cout << "Player " << w << " wins!" << std::endl;
                            gameOver = true;
                        } else if (placed == CELLS * CELLS) {
                            std::cout << "Cat's game -- it's a draw!" << std::endl;
                            gameOver = true;
                        } else {
                            current = current == 1 ? 2 : 1;
                        }
                    }
                }
            }
            wasPressed = pressed;
        }
        ge->update();
        draw->draw();
    }
    std::cout << "TIC TAC TOE CLOSED" << std::endl;
}
