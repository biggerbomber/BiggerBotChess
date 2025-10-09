#include "bitboard.h"

namespace BiggerBotChess
{

//helper function to print a bitboard
std::string print_bitboard(BitBoard bb) {
    std::string result;
    for (Rank r = RANK_1; r <= RANK_8; ++r) {
        for (File f = FILE_A; f <= FILE_H; ++f) {
            Square sq = get_square(r, f);
            result += (bb & (1ULL << sq)) ? square_to_str(sq) : " 0";
            result += ' ';
        }
        result += '\n';
    }
    return result;
}

} // namespace BiggerBotChess

