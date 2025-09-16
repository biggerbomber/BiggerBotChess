#include "move/move_generator.h"
#include "board/board.h"

namespace chess {

MoveGenerator::MoveGenerator() {
}

std::vector<Move> MoveGenerator::generateLegalMoves(const Board& board) const {
    std::vector<Move> pseudoLegalMoves = generatePseudoLegalMoves(board);
    std::vector<Move> legalMoves;
    
    // Filter out moves that leave king in check
    for (const Move& move : pseudoLegalMoves) {
        if (isLegalMove(board, move)) {
            legalMoves.push_back(move);
        }
    }
    
    return legalMoves;
}

std::vector<Move> MoveGenerator::generatePseudoLegalMoves(const Board& board) const {
    std::vector<Move> moves;
    
    Color sideToMove = board.getSideToMove();
    
    // Generate moves for all pieces of the side to move
    for (int row = 0; row < Board::BOARD_SIZE; ++row) {
        for (int col = 0; col < Board::BOARD_SIZE; ++col) {
            const Piece& piece = board.getPiece(row, col);
            
            if (piece.isEmpty() || piece.getColor() != sideToMove) {
                continue;
            }
            
            switch (piece.getType()) {
                case PieceType::Pawn:
                    generatePawnMoves(board, moves, row, col);
                    break;
                case PieceType::Knight:
                    generateKnightMoves(board, moves, row, col);
                    break;
                case PieceType::Bishop:
                    generateBishopMoves(board, moves, row, col);
                    break;
                case PieceType::Rook:
                    generateRookMoves(board, moves, row, col);
                    break;
                case PieceType::Queen:
                    generateQueenMoves(board, moves, row, col);
                    break;
                case PieceType::King:
                    generateKingMoves(board, moves, row, col);
                    break;
                default:
                    break;
            }
        }
    }
    
    return moves;
}

void MoveGenerator::generatePawnMoves(const Board& board, std::vector<Move>& moves, int row, int col) const {
    // TODO: Implement pawn move generation
    // This is a placeholder implementation
    Color color = board.getPiece(row, col).getColor();
    int direction = (color == Color::White) ? -1 : 1;
    
    // One square forward
    if (isValidSquare(row + direction, col) && board.getPiece(row + direction, col).isEmpty()) {
        moves.emplace_back(row, col, row + direction, col);
    }
}

void MoveGenerator::generateKnightMoves(const Board& board, std::vector<Move>& moves, int row, int col) const {
    // TODO: Implement knight move generation
    const int knightMoves[8][2] = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };
    
    Color color = board.getPiece(row, col).getColor();
    
    for (int i = 0; i < 8; ++i) {
        int newRow = row + knightMoves[i][0];
        int newCol = col + knightMoves[i][1];
        
        if (isValidSquare(newRow, newCol)) {
            const Piece& targetPiece = board.getPiece(newRow, newCol);
            if (targetPiece.isEmpty() || targetPiece.getColor() != color) {
                moves.emplace_back(row, col, newRow, newCol);
            }
        }
    }
}

void MoveGenerator::generateBishopMoves(const Board& board, std::vector<Move>& moves, int row, int col) const {
    // TODO: Implement bishop move generation
}

void MoveGenerator::generateRookMoves(const Board& board, std::vector<Move>& moves, int row, int col) const {
    // TODO: Implement rook move generation
}

void MoveGenerator::generateQueenMoves(const Board& board, std::vector<Move>& moves, int row, int col) const {
    // Queen moves like both rook and bishop
    generateRookMoves(board, moves, row, col);
    generateBishopMoves(board, moves, row, col);
}

void MoveGenerator::generateKingMoves(const Board& board, std::vector<Move>& moves, int row, int col) const {
    // TODO: Implement king move generation
}

void MoveGenerator::generateCastlingMoves(const Board& board, std::vector<Move>& moves) const {
    // TODO: Implement castling move generation
}

bool MoveGenerator::isValidSquare(int row, int col) const {
    return row >= 0 && row < Board::BOARD_SIZE && col >= 0 && col < Board::BOARD_SIZE;
}

bool MoveGenerator::isLegalMove(const Board& board, const Move& move) const {
    // TODO: Implement legal move validation
    // This should check if the move leaves the king in check
    return true; // Placeholder
}

} // namespace chess