#include "move/move.h"
#include "utils/utils.h"
#include <cctype>

namespace chess {

std::string Move::toAlgebraic() const {
    std::string from = Utils::coordsToSquare(m_fromRow, m_fromCol);
    std::string to = Utils::coordsToSquare(m_toRow, m_toCol);
    
    std::string result = from + to;
    
    // Add promotion piece if applicable
    if (m_promotionPiece > 0) {
        char promotionChars[] = { '\0', 'p', 'n', 'b', 'r', 'q', 'k' };
        result += promotionChars[m_promotionPiece];
    }
    
    return result;
}

Move Move::fromAlgebraic(const std::string& algebraic) {
    if (algebraic.length() < 4) {
        return Move(); // Invalid move
    }
    
    int fromRow, fromCol, toRow, toCol;
    
    if (!Utils::squareToCoords(algebraic.substr(0, 2), fromRow, fromCol) ||
        !Utils::squareToCoords(algebraic.substr(2, 2), toRow, toCol)) {
        return Move(); // Invalid squares
    }
    
    uint8_t promotionPiece = 0;
    if (algebraic.length() >= 5) {
        char p = std::tolower(algebraic[4]);
        switch (p) {
            case 'q': promotionPiece = 5; break;
            case 'r': promotionPiece = 4; break;
            case 'b': promotionPiece = 3; break;
            case 'n': promotionPiece = 2; break;
        }
    }
    
    return Move(fromRow, fromCol, toRow, toCol, promotionPiece);
}

bool Move::operator==(const Move& other) const {
    return m_fromRow == other.m_fromRow &&
           m_fromCol == other.m_fromCol &&
           m_toRow == other.m_toRow &&
           m_toCol == other.m_toCol &&
           m_promotionPiece == other.m_promotionPiece;
}

bool Move::operator!=(const Move& other) const {
    return !(*this == other);
}

} // namespace chess