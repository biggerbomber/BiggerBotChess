#include "search/search.h"
#include "board/board.h"
#include "move/move_generator.h"
#include "evaluation/evaluator.h"

namespace chess {

Search::Search(MoveGenerator* moveGen, Evaluator* eval)
    : m_moveGenerator(moveGen), m_evaluator(eval), m_bestScore(0), m_nodesSearched(0) {
}

Move Search::findBestMove(const Board& board, int depth) {
    m_nodesSearched = 0;
    m_bestScore = minimax(const_cast<Board&>(board), depth, -INFINITY_SCORE, INFINITY_SCORE, 
                         board.getSideToMove() == Color::White);
    return m_bestMove;
}

int Search::minimax(Board& board, int depth, int alpha, int beta, bool maximizing) {
    m_nodesSearched++;
    
    if (depth == 0) {
        return quiescenceSearch(board, alpha, beta, maximizing);
    }
    
    std::vector<Move> moves = m_moveGenerator->generateLegalMoves(board);
    
    if (moves.empty()) {
        // No legal moves - checkmate or stalemate
        if (board.isInCheck(board.getSideToMove())) {
            return maximizing ? -MATE_SCORE + (6 - depth) : MATE_SCORE - (6 - depth);
        } else {
            return 0; // Stalemate
        }
    }
    
    if (maximizing) {
        int maxEval = -INFINITY_SCORE;
        for (const Move& move : moves) {
            board.makeMove(move);
            int eval = minimax(board, depth - 1, alpha, beta, false);
            board.undoMove();
            
            if (eval > maxEval) {
                maxEval = eval;
                if (depth == 6) { // Original depth
                    m_bestMove = move;
                }
            }
            
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break; // Alpha-beta pruning
            }
        }
        return maxEval;
    } else {
        int minEval = INFINITY_SCORE;
        for (const Move& move : moves) {
            board.makeMove(move);
            int eval = minimax(board, depth - 1, alpha, beta, true);
            board.undoMove();
            
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break; // Alpha-beta pruning
            }
        }
        return minEval;
    }
}

int Search::quiescenceSearch(Board& board, int alpha, int beta, bool maximizing) {
    // Static evaluation
    int standPat = m_evaluator->evaluate(board);
    
    if (maximizing) {
        if (standPat >= beta) return beta;
        alpha = std::max(alpha, standPat);
    } else {
        if (standPat <= alpha) return alpha;
        beta = std::min(beta, standPat);
    }
    
    // TODO: Generate only capture moves for quiescence search
    // For now, just return static evaluation
    return standPat;
}

} // namespace chess