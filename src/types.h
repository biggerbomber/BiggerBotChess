#pragma once
#include <cstdint>
#include <cassert>
#include <string>

namespace BiggerBotChess {


enum Color : uint8_t {
    WHITE,
    BLACK,
    COLOR_NUM = 2
};


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
    RANK_NUM = 8,
    RANK_OVERFLOW = 255
};

enum File : uint8_t {
    FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H,
    FILE_FIRST = FILE_A,
    FILE_LAST = FILE_H,
    FILE_NUM = 8,
    FILE_OVERFLOW = 255
};


inline Color operator~(Color c){
    return static_cast<Color>(static_cast<int>(c) ^ 1);
}
inline Square& operator++(Square &s){
    return s = static_cast<Square>(static_cast<uint8_t>(s) + 1);
}
inline Rank& operator++(Rank &r){
    return r = static_cast<Rank>(static_cast<uint8_t>(r) + 1);
}
inline File& operator++(File &f){
    return f = static_cast<File>(static_cast<uint8_t>(f) + 1);
}

inline Square& operator--(Square &s){
    return s = static_cast<Square>(static_cast<uint8_t>(s) - 1);
}
inline Rank& operator--(Rank &r){
    return r = static_cast<Rank>(static_cast<uint8_t>(r) - 1);
}
inline File& operator--(File &f){
    return f = static_cast<File>(static_cast<uint8_t>(f) - 1);
}


inline Square get_square(Rank r, File f) {
    return static_cast<Square>(r * (uint8_t)8 + f);
}

inline Rank get_rank(Square sq) {
    return static_cast<Rank>(((uint8_t)sq / (uint8_t)8));
}

inline File get_file(Square sq) {
    return static_cast<File>(((uint8_t)sq % (uint8_t)8));
}

inline Square str_to_square(const std::string& str){
    assert(str.length() == 2);
    char file_char = str[0];
    char rank_char = str[1];

    assert(file_char >= 'a' && file_char <= 'h');
    assert(rank_char >= '1' && rank_char <= '8');

    File f = static_cast<File>(file_char - 'a');
    Rank r = static_cast<Rank>(rank_char - '1');

    return get_square(r, f);
}

inline std::string square_to_str(Square sq) {
    assert(sq >= S_FIRST && sq <= S_LAST);
    File f = get_file(sq);
    Rank r = get_rank(sq);
    return std::string(1, 'a' + static_cast<char>(f)) + std::string(1, '1' + static_cast<char>(r));
}


enum Castling : uint8_t {
    WHITE_KING_SIDE  = 1 << 0,
    WHITE_QUEEN_SIDE = 1 << 1,
    BLACK_KING_SIDE  = 1 << 2,
    BLACK_QUEEN_SIDE = 1 << 3,
    WHITE_SIDE = WHITE_KING_SIDE | WHITE_QUEEN_SIDE,
    BLACK_SIDE = BLACK_KING_SIDE | BLACK_QUEEN_SIDE,
    CASTLE_ALL       = WHITE_KING_SIDE | WHITE_QUEEN_SIDE | BLACK_KING_SIDE | BLACK_QUEEN_SIDE,
    CASTLE_NONE      = 0
};

inline Castling operator|(Castling a, Castling b) {
    return static_cast<Castling>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}
inline Castling& operator|=(Castling &a, Castling b) {
    a = static_cast<Castling>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
    return a;
}
inline Castling operator&(Castling a, Castling b) {
    return static_cast<Castling>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}
inline Castling& operator&=(Castling &a, Castling b) {
    a = static_cast<Castling>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
    return a;
}

enum Enpassant : uint8_t {
    EP_NONE = 0,
    EP_A2 = S_A2,
    EP_B2 = S_B2,
    EP_C2 = S_C2,
    EP_D2 = S_D2,
    EP_E2 = S_E2,
    EP_F2 = S_F2,
    EP_G2 = S_G2,
    EP_H2 = S_H2,
    EP_A7 = S_A7,
    EP_B7 = S_B7,
    EP_C7 = S_C7,
    EP_D7 = S_D7,
    EP_E7 = S_E7,
    EP_F7 = S_F7,
    EP_G7 = S_G7,
    EP_H7 = S_H7
};

enum PieceType {
    NONE,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    ALL_PIECES,
    PIECE_NUM = ALL_PIECES
};

constexpr uint8_t BLACK_PIECE_OFFSET = 7; 

enum Piece {
    NONE_PIECE,
    W_PAWN = PAWN,
    W_KNIGHT,
    W_BISHOP,
    W_ROOK,
    W_QUEEN,
    W_KING,
    B_PAWN = PAWN + (1 << BLACK_PIECE_OFFSET),
    B_KNIGHT,
    B_BISHOP,
    B_ROOK,
    B_QUEEN,
    B_KING
};

}// namespace BIggerBOtChess