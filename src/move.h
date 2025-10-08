#include "types.h"
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

    inline Square get_start(){
        return static_cast<Square>((m_data & (0x3FU << 6)) >> 6);
    };

    inline Square get_dest(){
        return static_cast<Square>(m_data & 0x3FU);
    };

    inline MoveType get_type(){
        return static_cast<MoveType>(m_data >> 14);
    };

    inline PieceType get_promotion_piece(){
        if(get_type() !=  PROMOTION){
            return NONE;
        }
        return static_cast<PieceType>(((m_data >> 12) & 0x3)+PIECE_CONV_OFSET);
    };

    Move(){
        m_data = 0;
    };
    Move(Square start, Square dest, MoveType type = NORMAL, PieceType prom_piece = KNIGHT){
        m_data = (type << 14) + ((prom_piece - PIECE_CONV_OFSET) << 12) + (start << 6) + dest;
    };

    uint16_t m_data = 0;
};



}//name space BiggerBotChess