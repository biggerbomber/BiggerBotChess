#pragma once
#include "types.h"
#include <string>
/*
Moves?
6 bit start
6 bit dest
12

16bit [extra, start, dest]
4 bit
prom piece 00 k  01 b 10 r  11 q
val -2 == > piece enum

00 normal 01 castle  10 promotion  11 enpassant 

*/
namespace BiggerBotChess {

enum MoveType{
    NORMAL,
    CASTLE,
    PROMOTION,
    EN_PASSANT
};

constexpr uint8_t PIECE_CONV_OFSET = 2;

class Move {
public:

    static Move null(){return Move();}
    inline Square get_start() const{
        return static_cast<Square>((m_data & (0x3FU << 6)) >> 6);
    };

    inline Square get_dest() const{
        return static_cast<Square>(m_data & 0x3FU);
    };

    inline MoveType get_type() const{
        return static_cast<MoveType>(m_data >> 14);
    };

    inline PieceType get_promotion_piece() const{
        if(get_type() !=  PROMOTION){
            return NONE;
        }
        return static_cast<PieceType>(((m_data >> 12) & 0x3)+PIECE_CONV_OFSET);
    };

    inline std::string to_str() const{
        std::string move_str = square_to_str(get_start()) + square_to_str(get_dest());
        if(get_type() == PROMOTION){
            switch (get_promotion_piece())
            {
                case KNIGHT: move_str += 'n'; break;
                case BISHOP: move_str += 'b'; break;
                case ROOK:   move_str += 'r'; break;
                case QUEEN:  move_str += 'q'; break;
                default: break;
            }
        }
        return move_str;
    };

    inline bool operator==(const Move& other){
        return m_data == other.m_data;
    };
   
    Move(Square start, Square dest, MoveType type = NORMAL, PieceType prom_piece = KNIGHT){
        m_data = (type << 14) + ((prom_piece - PIECE_CONV_OFSET) << 12) + (start << 6) + dest;
    };
    Move(){
        m_data = 0;
    };
    uint16_t m_data = 0;
};



}//name space BiggerBotChess