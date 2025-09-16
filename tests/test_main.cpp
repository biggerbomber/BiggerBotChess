#include <iostream>
#include <cassert>

// Simple test framework
#define TEST(name) void test_##name()
#define RUN_TEST(name) do { \
    std::cout << "Running " #name "... "; \
    test_##name(); \
    std::cout << "PASSED\n"; \
} while(0)

#define ASSERT_EQ(a, b) assert((a) == (b))
#define ASSERT_TRUE(a) assert(a)
#define ASSERT_FALSE(a) assert(!(a))

// Forward declarations
void test_board();
void test_move();
void test_utils();

int main() {
    std::cout << "Running BiggerBot Chess Engine Tests\n";
    std::cout << "====================================\n";
    
    try {
        RUN_TEST(board);
        RUN_TEST(move);
        RUN_TEST(utils);
        
        std::cout << "\nAll tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cout << "FAILED: " << e.what() << std::endl;
        return 1;
    }
}