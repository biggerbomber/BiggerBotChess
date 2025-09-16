#include <iostream>
#include "board.h"

int main() {
    using namespace BiggerBotChess;
    int a = 5;
    int b = 10;
    int sum = a + b;
    std::cout << "Hello, BiggerBotChess!" << std::endl;

    Board::init();

    std::cout << "Files Masks:" << std::endl;
    for (File i = FILE_FIRST; i <= FILE_LAST; ++i) {
        std::cout << "File " << static_cast<int>(i) << ":\n " << bitboard_print(Board::S_FILE_MASKS[i]) << std::endl;
    }

    return 0;
}
