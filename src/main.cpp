#include <iostream>
#include "board.h"
#include "move.h"
#include "movegenerator.h"

int main() {
    using namespace BiggerBotChess;

    std::cout << "Hello, BiggerBotChess!" << std::endl;

    Board::init();

    Board board("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    std::cout << board.get_board_info() << std::endl;
    std::cout << board.get_board_pretty_bb() << std::endl;
    std::cout << board.get_board_pretty() << std::endl;

    std::cout << sizeof(Board) << std::endl;


    BitBoard b = 0;
    BitBoard mask = get_mask(S_A1) | get_mask(S_A2) | get_mask(S_A3);
    do
        {
            b = (b - mask) & mask;
            std::cout << print_bitboard(b)<< std::endl;
        } while (b);
    


    return 0;
}
