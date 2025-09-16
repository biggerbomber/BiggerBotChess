#pragma once

#include "board/piece.h"

namespace chess {

class Board;

/**
 * @brief Evaluates chess positions
 */
class Evaluator {
public:
    Evaluator();
    
    /**
     * @brief Evaluate the current position
     * @param board Board to evaluate
     * @return Position evaluation (positive = white advantage, negative = black advantage)
     */
    int evaluate(const Board& board) const;

private:
    // Piece values in centipawns
    static constexpr int PAWN_VALUE = 100;
    static constexpr int KNIGHT_VALUE = 320;
    static constexpr int BISHOP_VALUE = 330;
    static constexpr int ROOK_VALUE = 500;
    static constexpr int QUEEN_VALUE = 900;
    static constexpr int KING_VALUE = 20000;
    
    int evaluateMaterial(const Board& board) const;
    int evaluatePosition(const Board& board) const;
    int evaluateKingSafety(const Board& board) const;
    int evaluatePawnStructure(const Board& board) const;
    
    int getPieceValue(PieceType type) const;
};

} // namespace chess