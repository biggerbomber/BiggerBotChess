#pragma once
#include <string>
#include "board.h"
#include <cstdint>


namespace BiggerBotChess {

class Test {
public:
    static void run_all();

    static void integrity_suite();
    static void run_integrity_test(const std::string& fen);

    static void perft_suite();
    static void perft_run(std::string fen, int depth, uint64_t expected_nodes);
private:
    static uint64_t perft(Board& board, int depth);
};

}