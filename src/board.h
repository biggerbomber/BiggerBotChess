#pragma once
#include <cstdint>
#include <string>

namespace BiggerBotChess {


using BitBoard = uint64_t;

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

enum Castling : uint8_t {
    WHITE_KING_SIDE  = 1 << 0,
    WHITE_QUEEN_SIDE = 1 << 1,
    BLACK_KING_SIDE  = 1 << 2,
    BLACK_QUEEN_SIDE = 1 << 3,
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
    EP_A2 = S_A2 << 1 + 1,
    EP_B2 = S_B2 << 1 + 1,
    EP_C2 = S_C2 << 1 + 1,
    EP_D2 = S_D2 << 1 + 1,
    EP_E2 = S_E2 << 1 + 1,
    EP_F2 = S_F2 << 1 + 1,
    EP_G2 = S_G2 << 1 + 1,
    EP_H2 = S_H2 << 1 + 1,
    EP_A7 = S_A7 << 1 + 1,
    EP_B7 = S_B7 << 1 + 1,
    EP_C7 = S_C7 << 1 + 1,
    EP_D7 = S_D7 << 1 + 1,
    EP_E7 = S_E7 << 1 + 1,
    EP_F7 = S_F7 << 1 + 1,
    EP_G7 = S_G7 << 1 + 1,
    EP_H7 = S_H7 << 1 + 1   
};


class Board {
public:
    // static things
    static void init();
    static BitBoard S_RANK_MASKS[RANK_NUM];
    static BitBoard S_FILE_MASKS[FILE_NUM];
    static BitBoard S_SQUARE_MASKS[S_NUM];
    constexpr static char  START_FEN [] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";


    Board(  const std::string& fen = START_FEN,
            const std::string& moves = "");

    void clear();

    std::string get_board_info() const;
    std::string get_board_pretty() const;


    BitBoard m_Occupancy = {0};
    BitBoard m_Pieces[COLOR_NUM] = {0};
    BitBoard m_Pawns[COLOR_NUM] = {0};
    BitBoard m_Knights[COLOR_NUM] = {0};
    BitBoard m_Bishops[COLOR_NUM] = {0};
    BitBoard m_Rooks[COLOR_NUM] = {0};
    BitBoard m_Queens[COLOR_NUM] = {0};
    BitBoard m_Kings[COLOR_NUM] = {0};
    Color m_ColorToMove = WHITE;
    
    // Castling rights
    Castling m_CastlingRights = CASTLE_NONE;

    Enpassant m_Enpassant = EP_NONE;
    uint16_t m_HalfmoveClock = 0;
    uint16_t m_FullmoveNumber = 0;
    
    
};

BitBoard rank_file_to_square_bb(Rank r, File f);
Square rank_file_to_square(Rank r, File f);
Rank square_to_rank(Square sq);
File square_to_file(Square sq);

Square str_to_square(const std::string& str);
std::string square_to_str(Square sq);

std::string bitboard_print(BitBoard bb);


} // namespace BiggerBotChess
