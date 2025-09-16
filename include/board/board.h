#pragma once

#include "board/piece.h"
#include <array>
#include <string>
#include <vector>

namespace chess {

class Move;

/**
 * @brief Represents a chess board position
 */
class Board {
public:
    static constexpr int BOARD_SIZE = 8;
    using BoardArray = std::array<std::array<Piece, BOARD_SIZE>, BOARD_SIZE>;

    Board();
    
    /**
     * @brief Initialize board from FEN string
     * @param fen FEN notation string
     */
    void setFromFen(const std::string& fen);
    
    /**
     * @brief Get current position as FEN string
     * @return FEN string
     */
    std::string toFen() const;
    
    /**
     * @brief Get piece at given square
     * @param row Row (0-7)
     * @param col Column (0-7)
     * @return Piece at that square
     */
    const Piece& getPiece(int row, int col) const;
    
    /**
     * @brief Set piece at given square
     * @param row Row (0-7)
     * @param col Column (0-7)
     * @param piece Piece to place
     */
    void setPiece(int row, int col, const Piece& piece);
    
    /**
     * @brief Make a move on the board
     * @param move Move to make
     * @return True if move was valid and made
     */
    bool makeMove(const Move& move);
    
    /**
     * @brief Undo the last move
     */
    void undoMove();
    
    /**
     * @brief Get the side to move
     * @return Color of side to move
     */
    Color getSideToMove() const { return m_sideToMove; }
    
    /**
     * @brief Check if position is in check
     * @param color Color to check
     * @return True if in check
     */
    bool isInCheck(Color color) const;
    
    /**
     * @brief Check if position is checkmate
     * @param color Color to check
     * @return True if checkmate
     */
    bool isCheckmate(Color color) const;
    
    /**
     * @brief Check if position is stalemate
     * @param color Color to check
     * @return True if stalemate
     */
    bool isStalemate(Color color) const;

private:
    BoardArray m_board;
    Color m_sideToMove;
    int m_halfmoveClock;
    int m_fullmoveNumber;
    
    // Castling rights
    bool m_whiteKingside;
    bool m_whiteQueenside;
    bool m_blackKingside;
    bool m_blackQueenside;
    
    // En passant target square
    int m_enPassantFile;
    
    void setupInitialPosition();
};

} // namespace chess