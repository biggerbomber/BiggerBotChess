#include "test.h"
#include "board.h"
#include "movegenerator.h"
#include <iostream>
#include <chrono>

namespace BiggerBotChess { 


auto fens = {
    std::string("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"),
    std::string("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 "),
    std::string("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"),
    std::string("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ")
};    

struct PerftTest {
        std::string fen;
        int depth;
        uint64_t expected_nodes;
};

std::vector<PerftTest> perft_tests = {
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ", 1, 20},
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ", 2, 400},
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ", 3, 8902},
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ", 4, 197281},
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ", 5, 4865609},
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ", 6, 119060324},
    {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 1, 48},
    {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 2, 2039},
    {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 3, 97862},
    {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 4, 4085603},
    {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 5, 193690690},
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 1, 14},
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 2, 191},
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 3, 2812},
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 4, 43238},
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 5, 674624},
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 6, 11030083},
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 1, 6},
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 2, 264},
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 3, 9467},
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 4, 422333},
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 5, 15833292},
    {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ", 1, 44},
    {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ", 2, 1486},
    {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ", 3, 62379},
    {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ", 4, 2103487},
    {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ", 5, 89941194}
};


void Test::run_all(){
    std::cout << "Starting test suites...\n";
    integrity_suite();
    perft_suite();
    std::cout << "All test suites completed.\n\n";
}



void Test::integrity_suite(){
    std::cout << "Running integrity suite...\n";

    for(const std::string& fen : fens){
        run_integrity_test(fen);
    }
    std::cout << "Integrity suite completed.\n";
}

void Test::run_integrity_test(const std::string& fen){
    Board board(fen);
    Board original_board = board;

    MoveSaver moves(board, LEGAL);

    for(const Move& m : moves){
        board.do_move(m,true);
        MoveSaver moves_2(board,LEGAL);
        for(const Move& m2 : moves_2){
            board.do_move(m2,true);
            MoveSaver moves_3(board,LEGAL);
            for(const Move& m3 : moves_3){
                
                board.do_move(m3,true);

                board.undo_move();
                board.sanity_check();
                board.undo_move();
                board.sanity_check();
                board.undo_move();
                board.sanity_check();

                if(board.get_board_info() != original_board.get_board_info()){
                    std::cout << "Integrity test failed for FEN: " << fen << "\n";
                    std::cout << "After moves: " << m.to_str() << ", " << m2.to_str() << ", " << m3.to_str() << "\n";  
                    return;
                }
                board.do_move(m,true);
                board.do_move(m2,true);
            }
            board.undo_move();
        }
        board.undo_move();
    }

    std::cout << "ok\n";
}

void Test::perft_suite(){
    std::cout << "Running perft suite...\n";

    for(const auto& test : perft_tests){
        perft_run(test.fen, test.depth, test.expected_nodes);
    }
    

    std::cout << "Perft suite completed.\n";
}

void Test::perft_run(std::string fen, int depth, uint64_t expected_nodes){
    Board board(fen);
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();
    
    uint64_t nodes = perft(board, depth);

    auto t2 = high_resolution_clock::now();

     //Getting number of milliseconds as an integer. 
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

     //Getting number of milliseconds as a double. 
    duration<double, std::milli> ms_double = t2 - t1;
    
    if(nodes != expected_nodes){
        std::cout << "Perft test failed for depth " << depth << " on FEN: " << fen << "\n";
        std::cout << "Expected: " << expected_nodes << ", Got: " << nodes << "\n";
    }else{
        std::cout << "ok :" << "depth: "<< depth << " time: "<<ms_int.count() << "ms\n";
    }
}

uint64_t Test::perft(Board& board, int depth){
    if(depth == 0){
        return 1;
    }

    uint64_t nodes = 0;
    MoveSaver moves(board, LEGAL);
    for(const Move& m : moves){
        board.do_move(m,true);
        nodes += perft(board, depth - 1);
        board.undo_move();
    }
    return nodes;
}
} // namespace BiggerBotChess