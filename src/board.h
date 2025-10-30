#pragma once
#include "types.h"
#include "move.h"
#include "bitboard.h"
#include "tt.h"
#include <vector>
#include <iostream>
#include <regex>
#include <array>

namespace BiggerBotChess {

struct BoardState{
    Move move;
    Piece capturedPiece;
    Castling castlingRights;
    Enpassant enpassant;
    uint16_t halfmoveClock;
    uint16_t fullplyNumber;
};

class Board {
public:
    // static things
    static void init();
    static Castling m_CastelingMask[S_NUM];

    constexpr static char  START_FEN [] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";


    Board(  const std::string& fen = START_FEN,
            const std::string& moves = "");

    Board(const Board& other) = delete;

    void clear();
    Key gen_zobrist_key() const;

    //General Info
    std::string get_board_info() const;
    std::string get_board_pretty_bb() const;
    std::string get_board_pretty() const;

    //Piece info Bitboard
    BitBoard get_pieces() const { return m_Occupancy;};
    inline BitBoard get_pieces(Color c, PieceType p = ALL_PIECES) const {
        const BitBoard * start = &m_Buffers[0];
        return start[static_cast<int>(p)*2 + static_cast<int>(c)];
    }
    inline BitBoard& get_pieces_ref(Color c, PieceType p){
        BitBoard * start = &m_Buffers[0];
        return start[static_cast<int>(p)*2 + static_cast<int>(c)];
    }

    inline Piece get_piece_on(Square s) const { return m_Board[s];};
    inline Color get_color() const{ return m_ColorToMove;};
    inline Enpassant get_enpassant() const { return m_StateHistory[m_StateHistoryIndex-1].enpassant;};

    inline Key get_key() const { return m_ZobristKey; }


    Piece make_piece(Color c, PieceType p);


    bool is_square_attacked(Square s, Color by) const;
    bool is_pseudo_legal(const Move& m);
    bool is_legal(const Move& m); 
    bool is_in_check(Color c) const;
    bool is_draw() const{
        BoardState st = m_StateHistory[m_StateHistoryIndex-1];
        return (st.halfmoveClock >= 100);
    }

    //for debugging, if the board is in a invalid state it will assert
    void sanity_check();

    //This function makes the move without checking legality
    void unsafe_do_move(const Move& m);
    
    bool do_move(const Move& m,bool trust_legal); //returns false if move is illegal

    //this function undoes the last move, uses the move list
    void undo_move();

    //this checks if the 2 squares are free and not attacked
    bool is_castle_possible(Color c, Castling side) const;

    void do_castle( Castling side, bool undo = false);

    void update_occupancy();




    //this funcions relay on the m_Board array being correct and in the previous state,
    //Modify The bitboard first then update m_Board accordingly
    inline void remove_piece_bb(Square sq){
        Piece p = get_piece_on(sq);
        Color c = get_color_of(p);
        BitBoard& pieces = get_pieces_ref(c,get_piece_type(p));

        pieces &= ~get_mask(sq);
        m_Pieces[c] &= ~get_mask(sq);
    }
    inline void put_piece_bb(Square sq, Piece p){
        //Piece p = get_piece_on(sq);
        Color c = get_color_of(p);
        BitBoard& pieces = get_pieces_ref(c,get_piece_type(p));

        pieces |= get_mask(sq);
        m_Pieces[c] |= get_mask(sq);
    }

    

    //Returns Move::null() if invalid or illegal
    Move str_to_move(const std::string& str);

    Color m_ColorToMove = WHITE;
    
    
    Piece m_Board [S_NUM];
    BitBoard m_Occupancy = {0};
    BitBoard m_Buffers[COLOR_NUM] = {0};
    BitBoard m_Pawns[COLOR_NUM] = {0};
    BitBoard m_Knights[COLOR_NUM] = {0};
    BitBoard m_Bishops[COLOR_NUM] = {0};
    BitBoard m_Rooks[COLOR_NUM] = {0};
    BitBoard m_Queens[COLOR_NUM] = {0};
    BitBoard m_Kings[COLOR_NUM] = {0};
    BitBoard m_Pieces[COLOR_NUM] = {0};

    //helper

    Key m_ZobristKey = 0;

    TT m_tt;

    std::array<BoardState,256> m_StateHistory;
    size_t m_StateHistoryIndex = 0;
};

} // namespace BiggerBotChess
