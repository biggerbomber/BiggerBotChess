#include "move/move.h"
#include <iostream>
#include <cassert>

#define ASSERT_EQ(a, b) assert((a) == (b))
#define ASSERT_TRUE(a) assert(a)

void test_move() {
    // Test move creation and conversion
    chess::Move move(6, 4, 4, 4); // e2 to e4
    
    ASSERT_EQ(move.getFromRow(), 6);
    ASSERT_EQ(move.getFromCol(), 4);
    ASSERT_EQ(move.getToRow(), 4);
    ASSERT_EQ(move.getToCol(), 4);
    
    // Test algebraic notation
    std::string algebraic = move.toAlgebraic();
    ASSERT_EQ(algebraic, "e2e4");
    
    // Test parsing from algebraic
    chess::Move parsedMove = chess::Move::fromAlgebraic("e2e4");
    ASSERT_EQ(parsedMove.getFromRow(), 6);
    ASSERT_EQ(parsedMove.getFromCol(), 4);
    ASSERT_EQ(parsedMove.getToRow(), 4);
    ASSERT_EQ(parsedMove.getToCol(), 4);
    
    // Test move equality
    ASSERT_TRUE(move == parsedMove);
}