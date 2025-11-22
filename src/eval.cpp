#include "eval.h"
#include "board.h"
#include "bitboard.h"

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

Evaluation features_eval(const Board& board) {
    Features features = get_features(board);
    double eval = BIAS;
    for(size_t i = 0; i < features.size(); ++i) {
        eval += WEIGHTS[i] * features[i];
    }
    if(board.get_color() == BLACK) {
        return -eval*100;
    }
    return eval*100;
}

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
        Square s_new = s;
        if(c == BLACK){
            //mirror rank
            File f = get_file(s);
            Rank r = get_rank(s);
            r = static_cast<Rank>(RANK_8 - r);
            s_new = get_square(r,f);
        }
        
        int pst_value = pst_middle[pt][s_new];
        if(c == WHITE){
            pst_score += pst_value;
        } else {
            pst_score -= pst_value;
        }

        if(pt == KING){
            //Use endgame table if low material
            int endgame_value = pst_king_endgame[s_new];
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
    eval += mobility_score*10;

    return (board.get_color() == WHITE) ? eval : -eval;
}

Evaluation eval(const Board& board) {
    return static_eval(board);
}

int get_mobility_score(BitBoard attacks){
    return count_bits(attacks);
}

int get_mobility_piece(const Board& board, PieceType pt, Color c){
    assert(pt != PAWN);
    BitBoard pos = board.get_pieces(c, pt);
    BitBoard total = 0;
    while(pos){
        Square s = pop_lsb(pos);
        BitBoard attacks = BB::get_attacks(s,pt,board.get_pieces()) & ~board.get_pieces(c);
        //std::cout << "Attacks for " << pt << ": " << square_to_str(s) <<" "<< print_bitboard(attacks) << "\n";
        total |= attacks;
    }
    //std::cout << "Total attacks for " << pt << ": " << print_bitboard(total) << "\n";
    return get_mobility_score(total);
}



void get_feature_color(const Board& board, Color c, Features& features){

    int * feat = &features[c * FEAT_COUNT];

    //Piece counts
    for(int pt = PAWN; pt <= KING; ++pt){
        BitBoard pieces = board.get_pieces(c,(PieceType)pt);
        int count = count_bits(pieces);
        switch(pt){
            case PAWN:
                feat[NUM_PAWNS] = count;
                break;
            case KNIGHT:
                feat[NUM_KNIGHTS] = count;
                break;
            case BISHOP:
                feat[NUM_BISHOPS] = count;
                break;
            case ROOK:
                feat[NUM_ROOKS] = count;
                break;
            case QUEEN:
                feat[NUM_QUEENS] = count;
                break;
            default:
                break;
        }
    }

    //Pawn structure
    BitBoard pawns = board.get_pieces(c,PAWN);
    BitBoard doubled_pawns = pawns & shift_bb(pawns, SOUTH);

    feat[DOUBLED_PAWNS] = count_bits(doubled_pawns);


    BitBoard old_pawns = pawns;
    BitBoard opp_pawns = board.get_pieces(~c,PAWN);

    while(pawns){
        Square square = pop_lsb(pawns);
        BitBoard file_mask = get_mask(get_file(square));
        BitBoard adjacent_files = shift_bb(file_mask, EAST) | shift_bb(file_mask, WEST);   
        BitBoard opp_pawns_on_files = opp_pawns & (file_mask | adjacent_files);
        BitBoard own_pawns_on_files = old_pawns & (file_mask | adjacent_files);
        //std::cout << "Pawn on " << square_to_str(square) << ": own pawns on files: \n" << print_bitboard(own_pawns_on_files) << ", opp pawns on files: \n" << print_bitboard(opp_pawns_on_files) << "\n";
        if(own_pawns_on_files == get_mask(square)){
            feat[ISOLATED_PAWNS] += 1;
        }
        if(opp_pawns_on_files == 0){
            feat[PASSED_PAWNS] += 1;
        }
    }

    //Piece mobility

    feat[KNIGHT_MOBILITY_NORM] = (feat[NUM_KNIGHTS] > 0) ? get_mobility_piece(board, KNIGHT, c) / feat[NUM_KNIGHTS] : 0;
    feat[BISHOP_MOBILITY_NORM] = (feat[NUM_BISHOPS] > 0) ? get_mobility_piece(board, BISHOP, c) / feat[NUM_BISHOPS] : 0;
    feat[ROOK_MOBILITY_NORM] = (feat[NUM_ROOKS] > 0) ? get_mobility_piece(board, ROOK, c) / feat[NUM_ROOKS] : 0;
    feat[QUEEN_MOBILITY_NORM] = (feat[NUM_QUEENS] > 0) ? get_mobility_piece(board, QUEEN, c) / feat[NUM_QUEENS] : 0;

    //king exposure
    //how many squares around king are attacked by opponent pieces
    BitBoard king_bb = board.get_pieces(c,KING);
    Direction directions[8] = {NORTH, EAST, SOUTH, WEST, NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST};
    for(int i = 0;i<8;++i){
        BitBoard adj_square_bb = shift_bb(king_bb, directions[i]);
        if(adj_square_bb == 0) continue;
        Square adj_square = lsb(adj_square_bb);

        bool attacked = board.is_square_attacked(adj_square, ~c);

        bool prot = board.is_square_attacked(adj_square, c);

        if(attacked && !prot){
            feat[KING_EXPOSURE] += 1;
        }
        if(prot && !attacked){
            feat[KING_PROTECTION] += 1;
        }
    }

}



Features get_features(const Board& board){
    Features features = {0};
    get_feature_color(board, WHITE, features);
    get_feature_color(board, BLACK, features);
    return features;
}

std::string features_to_str(const Features& features){
    std::string result;
    result += "White Features:\n";
    result += "Num Pawns: " + std::to_string(features[NUM_PAWNS]) + "\n";
    result += "Num Knights: " + std::to_string(features[NUM_KNIGHTS]) + "\n";
    result += "Num Bishops: " + std::to_string(features[NUM_BISHOPS]) + "\n";
    result += "Num Rooks: " + std::to_string(features[NUM_ROOKS]) + "\n";
    result += "Num Queens: " + std::to_string(features[NUM_QUEENS]) + "\n";
    result += "Doubled Pawns: " + std::to_string(features[DOUBLED_PAWNS]) + "\n";
    result += "Isolated Pawns: " + std::to_string(features[ISOLATED_PAWNS]) + "\n";
    result += "Passed Pawns: " + std::to_string(features[PASSED_PAWNS]) + "\n";
    result += "Knight Mobility Norm: " + std::to_string(features[KNIGHT_MOBILITY_NORM]) + "\n";
    result += "Bishop Mobility Norm: " + std::to_string(features[BISHOP_MOBILITY_NORM]) + "\n";
    result += "Rook Mobility Norm: " + std::to_string(features[ROOK_MOBILITY_NORM]) + "\n";
    result += "Queen Mobility Norm: " + std::to_string(features[QUEEN_MOBILITY_NORM]) + "\n";
    result += "King Exposure: " + std::to_string(features[KING_EXPOSURE]) + "\n";
    result += "King Protection: " + std::to_string(features[KING_PROTECTION]) + "\n";   
    result += "Black Features:\n";
    result += "Num Pawns: " + std::to_string(features[FEAT_COUNT + NUM_PAWNS]) + "\n";
    result += "Num Knights: " + std::to_string(features[FEAT_COUNT + NUM_KNIGHTS]) + "\n";
    result += "Num Bishops: " + std::to_string(features[FEAT_COUNT + NUM_BISHOPS]) + "\n";
    result += "Num Rooks: " + std::to_string(features[FEAT_COUNT + NUM_ROOKS]) + "\n";
    result += "Num Queens: " + std::to_string(features[FEAT_COUNT + NUM_QUEENS]) + "\n";
    result += "Doubled Pawns: " + std::to_string(features[FEAT_COUNT + DOUBLED_PAWNS]) + "\n";
    result += "Isolated Pawns: " + std::to_string(features[FEAT_COUNT + ISOLATED_PAWNS]) + "\n";
    result += "Passed Pawns: " + std::to_string(features[FEAT_COUNT + PASSED_PAWNS]) + "\n";    
    result += "Knight Mobility Norm: " + std::to_string(features[FEAT_COUNT + KNIGHT_MOBILITY_NORM]) + "\n";
    result += "Bishop Mobility Norm: " + std::to_string(features[FEAT_COUNT + BISHOP_MOBILITY_NORM]) + "\n";
    result += "Rook Mobility Norm: " + std::to_string(features[FEAT_COUNT + ROOK_MOBILITY_NORM]) + "\n";
    result += "Queen Mobility Norm: " + std::to_string(features[FEAT_COUNT + QUEEN_MOBILITY_NORM]) + "\n";
    result += "King Exposure: " + std::to_string(features[FEAT_COUNT + KING_EXPOSURE]) + "\n";
    result += "King Protection: " + std::to_string(features[FEAT_COUNT + KING_PROTECTION]) + "\n";
    return result;
}

std::string features_to_str_compact(const Features& features) {
    std::string result;
    for(size_t i = 0; i < features.size(); ++i) {
        result += std::to_string(features[i]);
        if (i != features.size() - 1) {
            result += ",";
        }
    }
    return result;


}

} // namespace Eval

} // namespace BiggerBotChess