#include "board/board.h"
#include "board/piece.h"
#include <iostream>
#include <cassert>

#define ASSERT_EQ(a, b) assert((a) == (b))
#define ASSERT_TRUE(a) assert(a)

void test_board() {
    chess::Board board;
    
    // Test initial position
    std::string initialFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    board.setFromFen(initialFen);
    
    // Test that we can get pieces from the board
    chess::Piece whiteRook = board.getPiece(7, 0);
    ASSERT_EQ(whiteRook.getType(), chess::PieceType::Rook);
    ASSERT_EQ(whiteRook.getColor(), chess::Color::White);
    
    chess::Piece blackRook = board.getPiece(0, 0);
    ASSERT_EQ(blackRook.getType(), chess::PieceType::Rook);
    ASSERT_EQ(blackRook.getColor(), chess::Color::Black);
    
    // Test empty square
    chess::Piece empty = board.getPiece(3, 3);
    ASSERT_TRUE(empty.isEmpty());
    
    // Test side to move
    ASSERT_EQ(board.getSideToMove(), chess::Color::White);
}