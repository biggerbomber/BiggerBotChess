#include "board.h"


namespace BiggerBotChess {

BitBoard Board::S_SQUARE_MASKS[S_NUM];
BitBoard Board::S_RANK_MASKS[RANK_NUM];
BitBoard Board::S_FILE_MASKS[FILE_NUM];

void Board::init() {
    // INIT SQUARE MASKS
    for (Square sq = S_FIRST; sq <= S_LAST; ++sq) {
        S_SQUARE_MASKS[sq] = (1ULL << sq);
    }
    // INIT RANK MASKS
    for (Rank r = RANK_FIRST; r <= RANK_LAST; ++r) {
        BitBoard rank_mask = (1ULL << 8) - 1;
        rank_mask <<= (r * 8);
        S_RANK_MASKS[r] = rank_mask;
    }
    // INIT FILE MASKS

    for (File f = FILE_FIRST; f <= FILE_LAST; ++f) {
        BitBoard file_mask = 0x101010101010101ULL;
        file_mask <<= f;
        S_FILE_MASKS[f] = file_mask;
    }
}

BitBoard rf_to_sq_bb(Rank r, File f) {
    return (1ULL << (r * (uint8_t)8 + f));
}

Square rf_to_sq(Rank r, File f) {
    return static_cast<Square>(r * (uint8_t)8 + f);
}

Rank sq_to_r(Square sq) {
    return static_cast<Rank>(((uint8_t)sq / (uint8_t)8));
}

File sq_to_f(Square sq) {
    return static_cast<File>(((uint8_t)sq % (uint8_t)8));
}

std::string bitboard_print(BitBoard bb) {
    std::string result;
    for (Rank r = RANK_1; r <= RANK_8; ++r) {
        for (File f = FILE_A; f <= FILE_H; ++f) {
            Square sq = rf_to_sq(r, f);
            result += (bb & (1ULL << sq)) ? '1' : '0';
            result += ' ';
        }
        result += '\n';
    }
    return result;
}

} // namespace BiggerBotChess