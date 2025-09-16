#include "evaluation/evaluator.h"
#include "board/board.h"

namespace chess {

Evaluator::Evaluator() {
}

int Evaluator::evaluate(const Board& board) const {
    int score = 0;
    
    score += evaluateMaterial(board);
    score += evaluatePosition(board);
    score += evaluateKingSafety(board);
    score += evaluatePawnStructure(board);
    
    return score;
}

int Evaluator::evaluateMaterial(const Board& board) const {
    int materialScore = 0;
    
    for (int row = 0; row < Board::BOARD_SIZE; ++row) {
        for (int col = 0; col < Board::BOARD_SIZE; ++col) {
            const Piece& piece = board.getPiece(row, col);
            
            if (!piece.isEmpty()) {
                int value = getPieceValue(piece.getType());
                materialScore += (piece.isWhite()) ? value : -value;
            }
        }
    }
    
    return materialScore;
}

int Evaluator::evaluatePosition(const Board& board) const {
    // TODO: Implement positional evaluation
    // Consider piece mobility, central control, etc.
    return 0;
}

int Evaluator::evaluateKingSafety(const Board& board) const {
    // TODO: Implement king safety evaluation
    // Consider pawn shield, king position, etc.
    return 0;
}

int Evaluator::evaluatePawnStructure(const Board& board) const {
    // TODO: Implement pawn structure evaluation
    // Consider doubled pawns, isolated pawns, passed pawns, etc.
    return 0;
}

int Evaluator::getPieceValue(PieceType type) const {
    switch (type) {
        case PieceType::Pawn:   return PAWN_VALUE;
        case PieceType::Knight: return KNIGHT_VALUE;
        case PieceType::Bishop: return BISHOP_VALUE;
        case PieceType::Rook:   return ROOK_VALUE;
        case PieceType::Queen:  return QUEEN_VALUE;
        case PieceType::King:   return KING_VALUE;
        default:                return 0;
    }
}

} // namespace chess