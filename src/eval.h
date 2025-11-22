#pragma once
#include "types.h"
#include <array>
#include <string>




namespace BiggerBotChess {
class Board;
namespace Eval {


using Evaluation = int;

constexpr Evaluation MAX_EVAL = 100000;
constexpr Evaluation MIN_EVAL = -MAX_EVAL;

//PLY
constexpr Evaluation MAX_MATE_DISTANCE = 200;

constexpr Evaluation MAX_LIMIT_MATE_SCORE = MAX_EVAL - MAX_MATE_DISTANCE;
constexpr Evaluation MIN_LIMIT_MATE_SCORE = -MAX_LIMIT_MATE_SCORE;

void init();
//Work in progress
enum Feat {
    NUM_PAWNS,
    NUM_KNIGHTS,
    NUM_BISHOPS,
    NUM_ROOKS,
    NUM_QUEENS,


    DOUBLED_PAWNS,
    ISOLATED_PAWNS,
    PASSED_PAWNS,
  
    
    KNIGHT_MOBILITY_NORM,
    BISHOP_MOBILITY_NORM,
    ROOK_MOBILITY_NORM,
    QUEEN_MOBILITY_NORM,

    KING_EXPOSURE,
    KING_PROTECTION,

    FEAT_COUNT,
    FEAT_TOTAL = FEAT_COUNT * 2 //for both colors
};

//2 mil positions, loss 0.65 04-11-2025

/*
]
Bias: [0.46438049]
*/
constexpr std::array<double, FEAT_TOTAL> WEIGHTS = { 1.85425521 , 4.30682376 , 5.01978324 , 7.49138223 , 12.65206967,
  -0.63240959,  -0.09277727,   0.99903286,   0.31202223,   0.18579826,
   0.32767863,   0.06291982,   0.,           0.41470506,  -1.83698534,
  -4.25494055,  -5.05520576,  -7.5023951,  -12.42113654,   0.80056248,
   0.10013117,  -0.8793193,   -0.08863576,  -0.17369071,  -0.49070281,
  -0.13207197,   0.,          -0.56148071};
constexpr double BIAS = 0.46438049;
using Features = std::array<int,FEAT_TOTAL>;
//Work in progress

Evaluation features_eval(const Board& board);

Features get_features(const Board& board);
std::string features_to_str(const Features& features);
std::string features_to_str_compact(const Features& features);




inline Evaluation mate_in_ply(int ply){
    return MAX_LIMIT_MATE_SCORE - ply;
}

inline bool is_mate(Evaluation eval){
    return eval > MAX_LIMIT_MATE_SCORE || eval < MIN_LIMIT_MATE_SCORE;
}




Evaluation static_eval(const Board& board);

//Main evaluation function, for now just calls static_eval
Evaluation eval(const Board& board);

} // namespace Eval
} // namespace BiggerBotChess