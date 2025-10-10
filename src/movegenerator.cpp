#include "movegenerator.h"
#include "bitboard.h"

namespace BiggerBotChess{

void gen_moves_pawns(Board& b,GenMoveType type, BitBoard target, MoveSaver* moves){
    b.clear();
    type = type; target = target; moves = moves;
};
void gen_moves_general(Board& b, PieceType type, BitBoard target, MoveSaver* moves){
    b.clear();
    type = type; target = target; moves = moves;
}

void i_generate(Board& b , GenMoveType genType, MoveSaver* moves){
    
    BitBoard target = (genType == CAPTURE) ? b.get_pieces(~b.get_color())
                    : (genType == STABLE)  ? ~b.get_pieces() 
                    : ~b.get_pieces(b.get_color());

    gen_moves_pawns(b,genType,target,moves);
    gen_moves_general(b,KNIGHT,target,moves);
    gen_moves_general(b,BISHOP,target,moves);
    gen_moves_general(b,ROOK,target,moves);
    gen_moves_general(b,QUEEN,target,moves);
    gen_moves_general(b,KING,target,moves);


    //Castling
    if(genType != CAPTURE || b.m_CastlingRights & (b.get_color() == WHITE ? WHITE_SIDE : BLACK_SIDE)){
        


    }
   
}


void generate_moves(Board& b, GenMoveType g, MoveSaver* moves){
    assert(g != LEGAL);
    switch (g)
    {
    case CAPTURE:   
    case STABLE:
        i_generate(b,g,moves);
        break;
    case ALL:
        generate_moves(b,CAPTURE,moves);
        generate_moves(b,STABLE,moves);
        break;
    default:
        break;
    }
}


void generate_moves_legal(Board& b, MoveSaver* moves);


}