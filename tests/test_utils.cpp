#include "utils/utils.h"
#include <iostream>
#include <cassert>

#define ASSERT_EQ(a, b) assert((a) == (b))
#define ASSERT_TRUE(a) assert(a)

void test_utils() {
    // Test square to coordinates conversion
    int row, col;
    ASSERT_TRUE(chess::Utils::squareToCoords("e4", row, col));
    ASSERT_EQ(row, 4);
    ASSERT_EQ(col, 4);
    
    ASSERT_TRUE(chess::Utils::squareToCoords("a1", row, col));
    ASSERT_EQ(row, 7);
    ASSERT_EQ(col, 0);
    
    ASSERT_TRUE(chess::Utils::squareToCoords("h8", row, col));
    ASSERT_EQ(row, 0);
    ASSERT_EQ(col, 7);
    
    // Test coordinates to square conversion
    ASSERT_EQ(chess::Utils::coordsToSquare(4, 4), "e4");
    ASSERT_EQ(chess::Utils::coordsToSquare(7, 0), "a1");
    ASSERT_EQ(chess::Utils::coordsToSquare(0, 7), "h8");
    
    // Test string splitting
    std::vector<std::string> parts = chess::Utils::split("a b c", ' ');
    ASSERT_EQ(parts.size(), 3);
    ASSERT_EQ(parts[0], "a");
    ASSERT_EQ(parts[1], "b");
    ASSERT_EQ(parts[2], "c");
}