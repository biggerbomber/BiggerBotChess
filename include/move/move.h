#pragma once

#include <cstdint>
#include <string>

namespace chess {

/**
 * @brief Represents a chess move
 */
class Move {
public:
    Move() : m_fromRow(0), m_fromCol(0), m_toRow(0), m_toCol(0), m_promotionPiece(0) {}
    
    Move(int fromRow, int fromCol, int toRow, int toCol, uint8_t promotionPiece = 0)
        : m_fromRow(fromRow), m_fromCol(fromCol), m_toRow(toRow), m_toCol(toCol), m_promotionPiece(promotionPiece) {}

    int getFromRow() const { return m_fromRow; }
    int getFromCol() const { return m_fromCol; }
    int getToRow() const { return m_toRow; }
    int getToCol() const { return m_toCol; }
    uint8_t getPromotionPiece() const { return m_promotionPiece; }

    /**
     * @brief Convert move to algebraic notation
     * @return Move in algebraic notation (e.g., "e2e4")
     */
    std::string toAlgebraic() const;

    /**
     * @brief Create move from algebraic notation
     * @param algebraic Move in algebraic notation
     * @return Move object
     */
    static Move fromAlgebraic(const std::string& algebraic);

    bool operator==(const Move& other) const;
    bool operator!=(const Move& other) const;

private:
    int m_fromRow, m_fromCol;
    int m_toRow, m_toCol;
    uint8_t m_promotionPiece;
};

} // namespace chess