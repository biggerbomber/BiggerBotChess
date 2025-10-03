#include <iostream>
#include "board.h"

int main() {
    using namespace BiggerBotChess;
    int a = 5;
    int b = 10;
    int sum = a + b;
    std::cout << "Hello, BiggerBotChess!" << std::endl;

    Board::init();

    Board board("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    std::cout << board.get_board_info() << std::endl;



    return 0;
}
