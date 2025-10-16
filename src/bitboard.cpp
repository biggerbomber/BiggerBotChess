#include "bitboard.h"
#include <iostream>
namespace BiggerBotChess
{

bool BB::s_IsInit = false;
BitBoard BB::s_PlainAttacks[PIECE_NUM][S_NUM] = {{0}};
BitBoard BB::s_PawnAttacks[COLOR_NUM][64] = {{0}};
BitBoard BB::s_BishopAttacks[S_NUM][512] = {{0}};
BitBoard BB::s_RookAttacks[S_NUM][4096] = {{0}};
Magic BB::s_Magics[S_NUM][2] = {{{0, 0, 0,0}}};


//chess programing wiki https://www.chessprogramming.org/Looking_for_Magics
uint64_t random_uint64() {
  uint64_t u1, u2, u3, u4;
  u1 = (uint64_t)(random()) & 0xFFFF; u2 = (uint64_t)(random()) & 0xFFFF;
  u3 = (uint64_t)(random()) & 0xFFFF; u4 = (uint64_t)(random()) & 0xFFFF;
  return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

uint64_t random_uint64_fewbits() {
  return random_uint64() & random_uint64() & random_uint64();
}


BitBoard move_safe(Square s, int step) {
    //Goofy implementation, but it works
    //Later i must refactor this
    BitBoard b = 0;
    Square ns = static_cast<Square>(static_cast<int>(s) + step);
    if (ns >= S_FIRST && ns <= S_LAST) {
        // Check for wrap-around on files
        File f1 = get_file(s);
        File f2 = get_file(ns);
        if (std::abs(static_cast<int>(f1) - static_cast<int>(f2)) <= 2 || step % 8 == 0) {
            b = get_mask(ns);
        }
    }
    return b;
}


void BB::init(){


    init_magics(BISHOP);
    init_magics(ROOK);

    for(Square sq = S_FIRST; sq<=S_LAST;++sq){

        s_PawnAttacks[WHITE][sq] = get_pawn_attacks(sq, WHITE);
        s_PawnAttacks[BLACK][sq] = get_pawn_attacks(sq, BLACK);


        int kingDir [] = {NORTH, NORTH_EAST, EAST, SOUTH_EAST, SOUTH, SOUTH_WEST, WEST, NORTH_WEST};

        for(int d : kingDir){
            s_PlainAttacks[KING][sq] |= move_safe(sq,d);
        }

        int knightDir [] = {-17, -15, -10, -6, 6, 10, 15, 17};
        for(int d : knightDir){
            s_PlainAttacks[KNIGHT][sq] |= move_safe(sq,d);
        }

        s_PlainAttacks[BISHOP][sq] = get_sliding_attacks(sq, BISHOP);
        s_PlainAttacks[ROOK][sq] = get_sliding_attacks(sq, ROOK);

        s_PlainAttacks[QUEEN][sq] = s_PlainAttacks[BISHOP][sq] | s_PlainAttacks[ROOK][sq];
    }


    s_IsInit = true;
}
void BB::init_magics(PieceType p){
    assert(p == BISHOP || p == ROOK);

    int off = p-BISHOP;
    int dim = (off == 0) ? 512 : 4096;
    int shift = (off == 0) ? 9 : 12;

    for(Square sq = S_FIRST;sq<=S_LAST;++sq){

        //std::cout << "Init magics for " << ((p==BISHOP) ? "BISHOP" : "ROOK") << " on square " << square_to_str(sq) << "\n";

        BitBoard edges = ((get_mask(FILE_A) | get_mask(FILE_H)) & ~get_mask(get_file(sq))) 
                       | ((get_mask(RANK_1) | get_mask(RANK_8)) & ~get_mask(get_rank(sq)));

        Magic& m = s_Magics[sq][off];
        m.mask = get_sliding_attacks(sq,p) & ~edges;
        m.shift = shift;
        m.attacks = (off == 0) ? s_BishopAttacks[sq] : s_RookAttacks[sq];
    

        BitBoard occupancy[dim];
        BitBoard attacks_correct[dim];

        //black magic to enumarate all occupancies
        BitBoard b = 0;
        int count = 0;
        do{
            occupancy[count] = b;
            attacks_correct[count] = get_sliding_attacks(sq,p,b);
            b = (b - 1) & m.mask;
            count++;
        }while (b);
        
        //find magic
        bool found = false;
        while (!found)
        {
            do{
                m.magic = random_uint64_fewbits();
            }while(__builtin_popcountll((m.magic * m.mask) >> (64-m.shift)) < 6);

            int i;
            bool visited[dim] = {0};
            for(i = 0; i<count;i++){
                int index = m.get_index(occupancy[i]);
                if(visited[index]){
                    break;
                } else{
                    visited[index] = true;
                    m.attacks[index] = attacks_correct[i];
                }

            }
            //std::cout << i << " / " << count << "\n";
            if(i == count){
                found = true;
            }
        }
        
    }
}





BitBoard BB::get_attacks(Square s, PieceType type, BitBoard occupancy){
    assert(s_IsInit);
    assert(type != PAWN);

    switch (type)
    {
    case BISHOP:
    case ROOK:
        return s_Magics[s][type-BISHOP].get_attacks(occupancy);
    case QUEEN:
        return s_Magics[s][0].get_attacks(occupancy) | s_Magics[s][1].get_attacks(occupancy);
    default:
        return s_PlainAttacks[type][s];
    }
}

BitBoard BB::get_sliding_attacks(Square s, PieceType p, BitBoard occupancy){
    assert(p == BISHOP || p == ROOK);

    BitBoard attacks = 0;

    Direction bishopDir [4] = {NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST};
    Direction rookDir [4] = {NORTH, EAST, SOUTH, WEST};

    for(Direction d : ((p == BISHOP) ? bishopDir : rookDir)){

        Square sq = s;
        BitBoard b = 0;
        while((b = move_safe(sq,d))){
            attacks |= b;
            sq+=d;
            if(b & occupancy){
                break;
            }
        }

    }
    return attacks;
}

BitBoard BB::get_pawn_attacks(Square s, Color c){
    if(s_IsInit){
        return s_PawnAttacks[c][s];
    }
    return ((c == WHITE) ? (move_safe(s,NORTH_EAST) | move_safe(s,NORTH_WEST))
                    : (move_safe(s,SOUTH_EAST) | move_safe(s,SOUTH_WEST)));
}

//helper function to print a bitboard
std::string print_bitboard(BitBoard bb) {
    std::string result;
    for (Rank ra = RANK_1; ra <= RANK_8; ++ra) {
        Rank r = static_cast<Rank>(RANK_8 - ra);
        for (File f = FILE_A; f <= FILE_H; ++f) {
            Square sq = get_square(r, f);
            result += (bb & get_mask(sq)) ? square_to_str(sq) : " 0";
            result += ' ';
        }
        result += '\n';
    }
    return result;
}

} // namespace BiggerBotChess

