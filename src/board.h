#pragma once
#include <cstdint>
#include <string>

namespace BiggerBotChess {


using BitBoard = uint64_t;

enum Square : uint8_t{
    S_A1, S_B1, S_C1, S_D1, S_E1, S_F1, S_G1, S_H1,
    S_A2, S_B2, S_C2, S_D2, S_E2, S_F2, S_G2, S_H2,
    S_A3, S_B3, S_C3, S_D3, S_E3, S_F3, S_G3, S_H3,
    S_A4, S_B4, S_C4, S_D4, S_E4, S_F4, S_G4, S_H4,
    S_A5, S_B5, S_C5, S_D5, S_E5, S_F5, S_G5, S_H5,
    S_A6, S_B6, S_C6, S_D6, S_E6, S_F6, S_G6, S_H6,
    S_A7, S_B7, S_C7, S_D7, S_E7, S_F7, S_G7, S_H7,
    S_A8, S_B8, S_C8, S_D8, S_E8, S_F8, S_G8, S_H8,

    S_FIRST = S_A1,
    S_LAST = S_H8,
    S_NUM = 64
};



enum Rank : uint8_t {
    RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8,
    RANK_FIRST = RANK_1,
    RANK_LAST = RANK_8,
    RANK_NUM = 8
};

enum File : uint8_t {
    FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H,
    FILE_FIRST = FILE_A,
    FILE_LAST = FILE_H,
    FILE_NUM = 8
};


class Board {
public:
    // static things
    static void init();
    static BitBoard S_RANK_MASKS[8];
    static BitBoard S_FILE_MASKS[8];
    static BitBoard S_SQUARE_MASKS[64];


    Board();
    Board(const std::string& fen, const std::string& moves = "");

    BitBoard Occupancy;
    BitBoard WhitePieces;
    BitBoard BlackPieces;
    BitBoard Pawns_White;
    BitBoard Pawns_Black;
    BitBoard Knights_White;
    BitBoard Knights_Black;
    BitBoard Bishops_White;
    BitBoard Bishops_Black;
    BitBoard Rooks_White;
    BitBoard Rooks_Black;
    BitBoard Queens_White;
    BitBoard Queens_Black;
    BitBoard King_White;
    BitBoard King_Black;





};

BitBoard rank_file_to_square(Rank r, File f);
Rank square_to_rank(Square sq);
File square_to_file(Square sq);

std::string bitboard_print(BitBoard bb);

inline Rank& operator++(Rank &r){
    return r = static_cast<Rank>(static_cast<uint8_t>(r) + 1);
}
inline File& operator++(File &f){
    return f = static_cast<File>(static_cast<uint8_t>(f) + 1);
}
inline Square& operator++(Square &s){
    return s = static_cast<Square>(static_cast<uint8_t>(s) + 1);
}

} // namespace BiggerBotChess
