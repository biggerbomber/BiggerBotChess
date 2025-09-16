#pragma once

#include <cstdint>
#include <string>
#include <array>

namespace chess {

/**
 * @brief Represents a chess piece type
 */
enum class PieceType : uint8_t {
    None = 0,
    Pawn = 1,
    Knight = 2,
    Bishop = 3,
    Rook = 4,
    Queen = 5,
    King = 6
};

/**
 * @brief Represents piece color
 */
enum class Color : uint8_t {
    White = 0,
    Black = 1
};

/**
 * @brief Represents a chess piece
 */
class Piece {
public:
    Piece() : m_type(PieceType::None), m_color(Color::White) {}
    Piece(PieceType type, Color color) : m_type(type), m_color(color) {}

    PieceType getType() const { return m_type; }
    Color getColor() const { return m_color; }
    
    bool isEmpty() const { return m_type == PieceType::None; }
    bool isWhite() const { return m_color == Color::White; }
    bool isBlack() const { return m_color == Color::Black; }

    char toChar() const;
    static Piece fromChar(char c);

private:
    PieceType m_type;
    Color m_color;
};

} // namespace chess