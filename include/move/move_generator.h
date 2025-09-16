#pragma once

#include "move/move.h"
#include <vector>

namespace chess {

class Board;

/**
 * @brief Generates legal moves for a given position
 */
class MoveGenerator {
public:
    MoveGenerator();
    
    /**
     * @brief Generate all legal moves for the current position
     * @param board Current board position
     * @return Vector of legal moves
     */
    std::vector<Move> generateLegalMoves(const Board& board) const;
    
    /**
     * @brief Generate all pseudo-legal moves (may leave king in check)
     * @param board Current board position
     * @return Vector of pseudo-legal moves
     */
    std::vector<Move> generatePseudoLegalMoves(const Board& board) const;

private:
    void generatePawnMoves(const Board& board, std::vector<Move>& moves, int row, int col) const;
    void generateKnightMoves(const Board& board, std::vector<Move>& moves, int row, int col) const;
    void generateBishopMoves(const Board& board, std::vector<Move>& moves, int row, int col) const;
    void generateRookMoves(const Board& board, std::vector<Move>& moves, int row, int col) const;
    void generateQueenMoves(const Board& board, std::vector<Move>& moves, int row, int col) const;
    void generateKingMoves(const Board& board, std::vector<Move>& moves, int row, int col) const;
    void generateCastlingMoves(const Board& board, std::vector<Move>& moves) const;
    
    bool isValidSquare(int row, int col) const;
    bool isLegalMove(const Board& board, const Move& move) const;
};

} // namespace chess