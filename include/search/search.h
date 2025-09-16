#pragma once

#include "move/move.h"
#include <limits>

namespace chess {

class Board;
class MoveGenerator;
class Evaluator;

/**
 * @brief Search algorithms for finding the best move
 */
class Search {
public:
    static constexpr int MATE_SCORE = 10000;
    static constexpr int INFINITY_SCORE = std::numeric_limits<int>::max();

    Search(MoveGenerator* moveGen, Evaluator* eval);
    
    /**
     * @brief Find the best move using minimax with alpha-beta pruning
     * @param board Current board position
     * @param depth Maximum search depth
     * @return Best move found
     */
    Move findBestMove(const Board& board, int depth);
    
    /**
     * @brief Get the evaluation of the best move found
     * @return Evaluation score
     */
    int getBestScore() const { return m_bestScore; }

private:
    int minimax(Board& board, int depth, int alpha, int beta, bool maximizing);
    int quiescenceSearch(Board& board, int alpha, int beta, bool maximizing);
    
    MoveGenerator* m_moveGenerator;
    Evaluator* m_evaluator;
    Move m_bestMove;
    int m_bestScore;
    int m_nodesSearched;
};

} // namespace chess