#include "movegenerator.h"

namespace BiggerBotChess{

void i_generate(Board& b, BitBoard target, MoveSaver* moves);


void generate_moves(Board& b, GenMoveType g, MoveSaver* moves){
    assert(g != LEGAL);
    BitBoard target = 0;
    switch (g)
    {
    case CAPTURE:
        target = b.get_pieces(~b.get_color(),ALL_PIECES);
        i_generate(b,target,moves);
        break;
    case STABLE:
        target = ~b.get_pieces(~b.get_color(),ALL_PIECES);
        i_generate(b,target,moves);
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