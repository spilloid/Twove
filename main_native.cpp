#include "./classes/games/Quoridor.h"
#include "./classes/games/Pong.h"
#include "./classes/games/TicTacToe.h"
#include <thread>
int main() {
    int sel = 0;
    bool keepGoing = true;
    while (keepGoing) {
        std::cout << "Which game would you like to play?" << std::endl;
        std::cout << "(0) Tic Tac Toe, (1) Pong, (2) Quoridor, or (3) Bye Bye Game" << std::endl;
        std::cin >> sel;
        std::shared_ptr<Game> g;
        switch (sel) {
            case 0:
                std::cout << "Starting Tic Tac Toe" << std::endl;
                g = std::make_shared<TicTacToe>();
                break;
            case 1:
                std::cout << "Starting Pong" << std::endl;
                g = std::make_shared<Pong>();
                break;
            case 2:
                std::cout << "Starting Quoridor" << std::endl;
                g = std::make_shared<Quoridor>();
                break;
            case 3:
                std::cout << "exiting...." << std::endl;
                keepGoing = false;
            default:
                std::cout << "HUH?????" << std::endl;
                break;
        }
        if (keepGoing)
            g->start();
    }
}