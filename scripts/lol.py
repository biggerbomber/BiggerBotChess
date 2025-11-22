
"""#pragma once
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



}//name space BiggerBotChess"""



#i need a function to convert the m_data to a string representation of the move

def move_to_str(m_data: int) -> str:
    start = (m_data >> 6) & 0x3F
    dest = m_data & 0x3F
    move_type = (m_data >> 14) & 0x3
    prom_piece = (m_data >> 12) & 0x3

    def square_to_str(square: int) -> str:
        file = chr((square % 8) + ord('a'))
        rank = str((square // 8) + 1)
        return file + rank

    move_str = square_to_str(start) + square_to_str(dest)

    if move_type == 2:  # PROMOTION
        if prom_piece == 0:
            move_str += 'n'
        elif prom_piece == 1:
            move_str += 'b'
        elif prom_piece == 2:
            move_str += 'r'
        elif prom_piece == 3:
            move_str += 'q'

    return move_str


while True:
    try:
        m_data_input = input("Enter m_data (or 'exit' to quit): ")
        if m_data_input.lower() == 'exit':
            break
        m_data = int(m_data_input)
        print("Move string:", move_to_str(m_data))
    except ValueError:
        print("Invalid input. Please enter a valid integer for m_data.")
