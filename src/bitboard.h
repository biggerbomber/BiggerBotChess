#pragma once
#include "types.h"
//the idea is to put here all the utils related to bitboards

namespace BiggerBotChess {

using BitBoard = uint64_t;

inline BitBoard get_mask(Square sq){
    return (1ULL << sq);
}
inline BitBoard get_mask(Rank r){
    return (0xFFULL << (r * 8));
}
inline BitBoard get_mask(File f){
    return (0x0101010101010101ULL << f);
}

inline BitBoard get_square_bb(Rank r, File f) {
    return (1ULL << (r * (uint8_t)8 + f));
}

std::string print_bitboard(BitBoard bb);


} // namespace BiggerBotChess

