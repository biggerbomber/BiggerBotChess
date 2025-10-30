#include "movegenerator.h"
#include <iostream>
#include "bitboard.h"

namespace BiggerBotChess{

inline void gen_promotions(BitBoard spot, MoveSaver* moves, Direction d){
    while (spot)
    {
        Square s = pop_lsb(spot);

        *(moves) += Move(s - d, s,PROMOTION,QUEEN);
        *(moves) += Move(s - d, s,PROMOTION,ROOK);
        *(moves) += Move(s - d, s,PROMOTION,BISHOP);
        *(moves) += Move(s - d, s,PROMOTION,KNIGHT);

    }
    
}

void gen_moves_pawns(const Board& b,GenMoveType type, BitBoard target, MoveSaver* moves){
    
    //BitBoard occ = b.get_pieces();
    Color c = b.get_color();

    Rank lastRank = (c==WHITE)? RANK_7 : RANK_2;
    Rank fourthRank = (c==WHITE)? RANK_4 : RANK_5;
    Direction forwardDir =  (c==WHITE)? NORTH : SOUTH;

    //separation between 7 rank and other pawn

    BitBoard allPawns = b.get_pieces(c,PAWN);

    BitBoard h7Pawns = allPawns & get_mask(lastRank);
    BitBoard otherPawns = allPawns & ~get_mask(lastRank);

    //std::cout << print_bitboard(otherPawns) << std::endl;
    //std::cout << print_bitboard(h7Pawns) << std::endl;
    //std::cout << print_bitboard(target) << std::endl;


    if(h7Pawns){
        //capture promotion
        if(type == CAPTURE){
            BitBoard cpromE = shift_bb(h7Pawns,forwardDir+EAST) & target;
            gen_promotions(cpromE,moves,forwardDir+EAST);

            BitBoard cpromW = shift_bb(h7Pawns,forwardDir+WEST) & target;
            gen_promotions(cpromW,moves,forwardDir+WEST);
        }else{
            //non capture
            BitBoard prom = shift_bb(h7Pawns,forwardDir) & target;
            gen_promotions(prom,moves,forwardDir);
        }
    }

    if(otherPawns){
        if(type == CAPTURE){
            BitBoard captureE = shift_bb(otherPawns,forwardDir+EAST) & target;

            while (captureE)
            {
                Square s = pop_lsb(captureE);
                
                *(moves) += Move(s-(forwardDir+EAST),s);
            }

            BitBoard captureW = shift_bb(otherPawns,forwardDir+WEST) & target;
            
            while (captureW)
            {
                Square s = pop_lsb(captureW);
                
                *(moves) += Move(s-(forwardDir+WEST),s);
            }

            if(b.get_enpassant() != EP_NONE){
                BitBoard epTarget = get_mask(static_cast<Square>(b.get_enpassant()));
                //std::cout << "ep target: \n" << print_bitboard(epTarget) << std::endl;
                BitBoard epE = shift_bb(otherPawns,forwardDir+EAST) & epTarget;
                //std::cout << "epE: \n" << print_bitboard(epE) << std::endl;
                while (epE)
                {
                    Square s = pop_lsb(epE);
                    
                    *(moves) += Move(s-(forwardDir+EAST),s,EN_PASSANT);
                }

                BitBoard epW = shift_bb(otherPawns,forwardDir+WEST) & epTarget;
                //std::cout << "epW: \n" << print_bitboard(epW) << std::endl;
                while (epW)
                {
                    Square s = pop_lsb(epW);
                    
                    *(moves) += Move(s-(forwardDir+WEST),s,EN_PASSANT);
                }
            }

        }else{
            BitBoard push = shift_bb(otherPawns,forwardDir) & target;
            BitBoard doublePush = shift_bb(push,forwardDir) & target & get_mask(fourthRank);

            //std::cout << "bitboard push: \n" << print_bitboard(push) << std::endl;
            //std::cout << "bitboard doublePush: \n" << print_bitboard(doublePush) << std::endl;

            while (push)
            {
                Square s = pop_lsb(push);
                
                *(moves) += Move(s-(forwardDir),s);
            }
            while (doublePush)
            {
                Square s = pop_lsb(doublePush);
                
                *(moves) += Move(s-(forwardDir)-(forwardDir),s);
            }


        }


    }




};

template<PieceType type>
void gen_moves_general(const Board& b, BitBoard target, MoveSaver* moves){
    BitBoard occ = b.get_pieces();
    Color c = b.get_color();

    BitBoard pieces = b.get_pieces(c,type);

    while(pieces){

        Square start = pop_lsb(pieces);

        BitBoard attacks = BB::get_attacks<type>(start,occ) & target;

        while (attacks)
        {
            Square dest = pop_lsb(attacks);
            *(moves) += Move(start, dest);
        }
        
    }
}

void i_generate(const Board& b , GenMoveType genType, MoveSaver* moves){
    
    BitBoard target = (genType == CAPTURE) ? b.get_pieces(~b.get_color())
                    : (genType == STABLE)  ? ~b.get_pieces() 
                    : ~b.get_pieces(b.get_color());

    gen_moves_pawns(b,genType,target,moves);
    gen_moves_general<KNIGHT>(b,target,moves);
    gen_moves_general<BISHOP>(b,target,moves);
    gen_moves_general<ROOK>(b,target,moves);
    gen_moves_general<QUEEN>(b,target,moves);
    gen_moves_general<KING>(b,target,moves);


    //Castling
    if(genType != CAPTURE){ 
        if(b.is_castle_possible(b.get_color(),KING_SIDE)){
            Square from = (b.get_color() == WHITE) ? S_E1 : S_E8;
            Square to   = (b.get_color() == WHITE) ? S_G1 : S_G8;
            *(moves) += Move(from,to,CASTLE);
        }
        if(b.is_castle_possible(b.get_color(),QUEEN_SIDE)){
            Square from = (b.get_color() == WHITE) ? S_E1 : S_E8;
            Square to   = (b.get_color() == WHITE) ? S_C1 : S_C8;
            *(moves) += Move(from,to,CASTLE);
        }

    }
   
}


void generate_moves(const Board& b, const GenMoveType& g, MoveSaver* moves){
    assert(g != LEGAL);
    switch (g)
    {
    case CAPTURE:   
    case STABLE:
        i_generate(b,g,moves);
        if(g == CAPTURE){
            moves->m_CaptureSize = moves->m_free_pos;
        }
        break;
    case ALL:
        generate_moves(b,CAPTURE,moves);
        generate_moves(b,STABLE,moves);
        break;
    default:
        break;
    }
}


void generate_moves_legal(Board& b, MoveSaver* moves){
    MoveSaver pseudo_moves(b,ALL);
    for(const Move& m : pseudo_moves){
        if(b.is_legal(m)){
            *(moves) += m;
        }
    }
}


void MoveSaver::order_capture(){
    if(m_CaptureSize == 0 || m_CaptureSize >= m_free_pos){
        return;
    }
    std::sort(&m_data[m_Start], &m_data[m_CaptureSize], [this](const Move& lhs, const Move& rhs){
        const Board &b = *(this->m_Board);

        if(lhs.get_dest() != rhs.get_dest()){
            PieceType pt_lhs = get_piece_type(b.get_piece_on(lhs.get_dest()));
            PieceType pt_rhs = get_piece_type(b.get_piece_on(rhs.get_dest()));

            return pt_lhs > pt_rhs;
        }

        PieceType pt_lhs = get_piece_type(b.get_piece_on(lhs.get_start()));
        PieceType pt_rhs = get_piece_type(b.get_piece_on(rhs.get_start()));
        return pt_lhs < pt_rhs; 
    });

}

}