#pragma once
#include <string>
#include "board.h"
#include <cstdint>


namespace BiggerBotChess {

class Test {
public:
    static void run_all();

    static void random_path_integrity_test();
    static void run_path_integrity_test(const std::string& fen,int num_moves);

    static void integrity_suite();
    static void run_integrity_test(const std::string& fen);

    static void zobrist_suite();
    static void run_zobrist_test_random(const std::string& fen,int num_moves);

    static void perft_suite();
    static void perft_run(std::string fen, int depth, uint64_t expected_nodes);


    static void full_challenge();
private:
    static uint64_t perft(Board& board, int depth);
};

}