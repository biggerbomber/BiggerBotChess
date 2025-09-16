#include "board/piece.h"
#include <cctype>

namespace chess {

char Piece::toChar() const {
    if (isEmpty()) return '.';
    
    char piece_chars[] = { '.', 'p', 'n', 'b', 'r', 'q', 'k' };
    char c = piece_chars[static_cast<int>(m_type)];
    
    if (isWhite()) {
        c = std::toupper(c);
    }
    
    return c;
}

Piece Piece::fromChar(char c) {
    Color color = std::isupper(c) ? Color::White : Color::Black;
    c = std::tolower(c);
    
    PieceType type = PieceType::None;
    switch (c) {
        case 'p': type = PieceType::Pawn; break;
        case 'n': type = PieceType::Knight; break;
        case 'b': type = PieceType::Bishop; break;
        case 'r': type = PieceType::Rook; break;
        case 'q': type = PieceType::Queen; break;
        case 'k': type = PieceType::King; break;
        default: type = PieceType::None; break;
    }
    
    return Piece(type, color);
}

} // namespace chess