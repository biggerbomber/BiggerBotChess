#include "eval.h"
#include "board.h"


namespace BiggerBotChess {

namespace Eval {

const int piece_values[PIECE_NUM] = {0,100,320,330,500,900,0};
int pst_middle[PIECE_NUM][S_NUM] ={
    // NONE
    {
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0
    },
    // PAWN
    {
         0,  0,  0,  0,  0,  0,  0,  0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
        5,  5, 10, 25, 25, 10,  5,  5,
        0,  0,  0, 20, 20,  0,  0,  0,
        5, -5,-10,  0,  0,-10, -5,  5,
        5, 10, 10,-20,-20, 10, 10,  5,
        0,  0,  0,  0,  0,  0,  0,  0
    },
    // KNIGHT
    {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20 ,  0 ,  5 ,  5 ,  0 ,-20,-40,
        -30 , 5 ,10 ,15 ,15 ,10 ,5 ,-30,
        -30 , 0 ,15 ,20 ,20 ,15 ,0 ,-30,
        -30 , 5 ,15 ,20 ,20 ,15 ,5 ,-30,
        -30 , 0 ,10 ,15 ,15 ,10 ,0 ,-30,
        -40,-20 ,  0 ,  5 ,  5 ,  0 ,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50
    },
    // BISHOP
    {
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -20,-10,-10,-10,-10,-10,-10,-20
    },
    // ROOK
    {
        0 ,0 ,0 ,0 ,0 ,0 ,0 ,0,
        5 ,10,10,10,10,10,10,5,
        -5 ,0 ,0 ,0 ,0 ,0 ,0 ,-5,
        -5 ,0 ,0 ,0 ,0 ,0 ,0 ,-5,
        -5 ,0 ,0 ,0 ,0 ,0 ,0 ,-5,
        -5 ,0 ,0 ,0 ,0 ,0 ,0 ,-5,
        -5 ,0 ,0 ,0 ,0 ,0 ,0 ,-5,
        0 ,0 ,0 ,5 ,5 ,0 ,0 ,0
    },
    // QUEEN
    {
        -20,-10,-10, -5, -5,-10,-10,-20,
        -10 , 0 , 0 , 0 , 0 , 0 , 0 ,-10,
        -10 , 0 , 5 , 5 , 5 , 5 , 0 ,-10,
        -5 , 0 , 5 , 5 , 5 , 5 , 0 ,-5,
        0 , 0 , 5 , 5 , 5 , 5 , 0 ,-5,
        -10 , 5 , 5 , 5 , 5 , 5 , 0 ,-10,
        -10 , 0 , 0 , 0 , 0 , 0 , 0 ,-10,
        -20,-10,-10, -5, -5,-10,-10,-20
    },
    // KING
    {
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -20,-30,-30,-40,-40,-30,-30,-20,
        -10,-20,-20,-20,-20,-20,-20,-10,
        20 ,20 , 0 , 0 , 0 , 0 ,20 ,20,
        20 ,30 ,10 , 0 , 0 ,10 ,30 ,20
    }
};


int pst_king_endgame[64] = {
    -50,-40,-30,-20,-20,-30,-40,-50,
    -30,-20,-10,  0,  0,-10,-20,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-30,  0,  0,  0,  0,-30,-30,
    -50,-30,-30,-30,-30,-30,-30,-50
};

void init(){
    for(int i  = 0;i<PIECE_NUM;++i){
        for(int j = 0; j<=4;++j){
            for(int k = 0;k<8;++k){
                std::swap(pst_middle[i][j*8 + k], pst_middle[i][(7 - j)*8 + k]);
            }
        }
    }
}


//Evaluation features:
// 1 - raw material balance
// 2 - piece activity [precomputed tables of piece-square values]
// 3 - piece mobility
// to do:
// 5 - square control
// 6 - king safety (number of squares around king attacked by opponent pieces)
// 7 - pawn structure (doubled, passed)
// 8 - piece protectedness (by piece of lower value)



Evaluation static_eval(const Board& board) {
    Evaluation eval = 0;

    // 1- Raw material balance
    int total_material = 0;
    int material_score = 0;
    int pst_score = 0;
    int pst_king_score = 0;
    int mobility_score = 0;
    for(Square s = S_FIRST; s <= S_LAST; ++s){
        Piece p = board.get_piece_on(s);
        if(p == NONE_PIECE) continue;
        
        Color c = get_color_of(p);
        PieceType pt = get_piece_type(p);
        total_material += piece_values[pt];
        if(c == WHITE){
            material_score += piece_values[pt];
        } else {
            material_score -= piece_values[pt];
        }


        //2 - Piece activity 

        int pst_value = pst_middle[pt][s];
        if(c == WHITE){
            pst_score += pst_value;
        } else {
            pst_score -= pst_value;
        }

        if(pt == KING){
            //Use endgame table if low material
            int endgame_value = pst_king_endgame[s];
            if(c == WHITE){
                pst_king_score += endgame_value - pst_value;
            } else {
                pst_king_score -= endgame_value + pst_value;
            }
        }    

        //3 - Piece mobility 
        if(pt == PAWN) continue; //skip pawns
        BitBoard attacks = BB::get_attacks(s,pt,board.get_pieces());
        int mobility = count_bits(attacks);

        if(c == WHITE){
            mobility_score += mobility;
        } else {
            mobility_score -= mobility;
        }

    }
    eval += material_score;
    eval += pst_score;
    //eval += (total_material*pst_score + (8000 - total_material)*(pst_score + pst_king_score))/32000;
    //eval += mobility_score;

    return (board.get_color() == WHITE) ? eval : -eval;
}

Evaluation eval(const Board& board) {
    return static_eval(board);
}




/*
//Work in progress
Features get_features(const Board& board){
    Features features = {0};

    // Example feature: material count
    int white_material = 0;
    int black_material = 0;
    const int piece_values[PIECE_NUM] = {0,100,320,330,500,900,0};

    for(Square s = S_FIRST; s <= S_LAST; ++s){
        Piece p = board.get_piece_on(s);
        if(p != NONE_PIECE){
            Color c = get_color_of(p);
            PieceType pt = get_piece_type(p);
            if(c == WHITE){
                white_material += piece_values[pt];
            } else {
                black_material += piece_values[pt];
            }
        }


        
    }

    features[0] = white_material - black_material; // Material balance

    // Additional features can be added here

    return features;

}*/
} // namespace Eval

} // namespace BiggerBotChess