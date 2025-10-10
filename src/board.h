#pragma once
#include "types.h"
#include "move.h"
#include "bitboard.h"

namespace BiggerBotChess {

class Board {
public:
    // static things
    static void init();

    constexpr static char  START_FEN [] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";


    Board(  const std::string& fen = START_FEN,
            const std::string& moves = "");

    void clear();

    //General Info
    std::string get_board_info() const;
    std::string get_board_pretty_bb() const;
    std::string get_board_pretty() const;

    //Piece info Bitboard
    BitBoard get_pieces() { return m_Occupancy;};
    BitBoard get_pieces(Color c, PieceType p = ALL_PIECES);
    Color get_color() const{ return m_ColorToMove;};

    Piece make_piece(Color c, PieceType p);


    bool is_square_attacked(Square s, Color by);



    Piece m_Board [S_NUM];
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

} // namespace BiggerBotChess
