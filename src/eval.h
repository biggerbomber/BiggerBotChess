#pragma once
#include "types.h"






namespace BiggerBotChess {
class Board;
namespace Eval {

void init();
//Work in progress
enum Feat {
    MATERIAL,

    KING_FEAR,
    KING_PROTECTION,

    DOUBLED_PAWNS,
    ISOLATED_PAWNS,
    PASSED_PAWNS,

    PIECE_ACTIVITY,
    CONTROLLED_SQUARES,
    CENTER_CONTROL,
    DEVELOPMENT,
    CASTLING,
    TACTICAL_VULNERABILITY,
    PIECE_COORDINATION,
    SPACE,
    
    ENDGAME_PIECE_VALUE,
    FEAT_COUNT
};
//Work in progress/*

/*
constexpr std::array<double, FEAT_COUNT * 2> WEIGHTS = { 0.0 };  
using Features = std::array<int,FEAT_COUNT * 2>;
//Work in progress
Features get_features(const Board& board);*/



using Evaluation = int;

constexpr Evaluation MAX_EVAL = 100000;
constexpr Evaluation MIN_EVAL = -MAX_EVAL;

//PLY
constexpr Evaluation MAX_MATE_DISTANCE = 200;

constexpr Evaluation MAX_LIMIT_MATE_SCORE = MAX_EVAL - MAX_MATE_DISTANCE;
constexpr Evaluation MIN_LIMIT_MATE_SCORE = -MAX_LIMIT_MATE_SCORE;

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