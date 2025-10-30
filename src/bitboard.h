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

enum Direction: int{
    NORTH = 8,
    EAST = -1,
    SOUTH = -8,
    WEST = 1,
    NORTH_EAST = NORTH + EAST,
    NORTH_WEST = NORTH + WEST,
    SOUTH_EAST = SOUTH + EAST,
    SOUTH_WEST = SOUTH + WEST
};
inline Direction operator+(Direction s, Direction d){
    return static_cast<Direction>(static_cast<int>(s) + d);
}

inline Square& operator+=(Square& sq, Direction d){
    return sq = static_cast<Square>(static_cast<uint8_t>(sq) + d);
}

inline Square operator+(Square sq, Direction d){
    return static_cast<Square>(static_cast<uint8_t>(sq) + d);
}
inline Square operator-(Square sq, Direction d){
    return static_cast<Square>(static_cast<uint8_t>(sq) - d);
}


inline BitBoard shift_bb(BitBoard b , Direction d){
    switch (d)
    {
    case NORTH:
        return b << 8;
    case EAST:
        return (b & ~get_mask(FILE_A)) >> 1;
    case SOUTH:
        return b >> 8;
    case WEST:
        return (b & ~get_mask(FILE_H)) << 1;
    case NORTH_EAST:
        return shift_bb(shift_bb(b,NORTH),EAST);
    case NORTH_WEST:
        return shift_bb(shift_bb(b,NORTH),WEST);
    case SOUTH_EAST:
        return shift_bb(shift_bb(b,SOUTH),EAST);
    case SOUTH_WEST:
        return shift_bb(shift_bb(b,SOUTH),WEST);
    }
    assert(false);
    return 0;
}

inline Square lsb(const BitBoard& b){
    assert(b != 0);
    return static_cast<Square>(__builtin_ctzll(b));
}

inline Square pop_lsb(BitBoard& b){
    Square s = lsb(b);
    b &= ~get_mask(s);
    return s;
}

inline int pop_count(BitBoard b){
    return __builtin_popcountll(b);
}

BitBoard move_safe(Square s, int step);

struct Magic
{
    BitBoard mask;
    BitBoard magic;
    int shift;
    BitBoard * attacks;

    int get_index(BitBoard occupancy) const {
        return (int)(((occupancy & mask) * magic) >> (64 - shift));
    }

    BitBoard get_attacks(BitBoard occupancy) const{
        return attacks[get_index(occupancy)];
    }
};




class BB{
    public:

    static void init();
    static void init_magics(PieceType p);

    template<PieceType type>
    inline static BitBoard get_attacks(Square s, BitBoard occupancy = 0);

    static BitBoard get_attacks(Square s, PieceType type, BitBoard occupancy);

    static BitBoard get_sliding_attacks(Square s, PieceType p, BitBoard occupancy = 0);
    static BitBoard get_pawn_attacks(Square s, Color c);




    static bool s_IsInit;
    static BitBoard s_PlainAttacks[PIECE_NUM][S_NUM];
    static BitBoard s_PawnAttacks[COLOR_NUM][64];

    static BitBoard s_BishopAttacks[S_NUM][512];
    static BitBoard s_RookAttacks[S_NUM][4096];

    static Magic s_Magics[S_NUM][2]; //0 bishop, 1 rook

    

};


template<PieceType type>
inline BitBoard BB::get_attacks(Square s, BitBoard occupancy){
    assert(s_IsInit);
    assert(type != PAWN);

    switch (type)
    {
    case BISHOP:
    case ROOK:
        return s_Magics[s][type-BISHOP].get_attacks(occupancy);
    case QUEEN:
        return s_Magics[s][0].get_attacks(occupancy) | s_Magics[s][1].get_attacks(occupancy);
    default:
        return s_PlainAttacks[type][s];
    }
}

std::string print_bitboard(BitBoard bb);







} // namespace BiggerBotChess

