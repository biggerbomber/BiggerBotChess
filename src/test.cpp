#include "test.h"
#include "board.h"
#include "movegenerator.h"
#include "search.h"
#include <iostream>
#include <chrono>

namespace BiggerBotChess { 


auto fens = {
    std::string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "),
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
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ", 7, 3195901860},
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
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 7, 178633661},
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 8, 3009794393},
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 1, 6},
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 2, 264},
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 3, 9467},
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 4, 422333},
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 5, 15833292},
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 6, 706045033},

    {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ", 1, 44},
    {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ", 2, 1486},
    {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ", 3, 62379},
    {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ", 4, 2103487},
    {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ", 5, 89941194},
    {"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 ", 1, 46},
    {"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 ", 2, 2079},
    {"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 ", 3, 89890},
    {"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 ", 4, 3894594},
    {"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 ", 5, 164075551},

    

    
};

void Test::random_path_integrity_test(){
    std::cout << "Running random path integrity tests...\n";
    int max_moves = 20;

    for(auto& fen : fens){
        for(int i=0; i < max_moves; ++i){
            run_path_integrity_test(fen, i);
        }

        std::cout << "ok\n";
    }
    std::cout << "Random path integrity tests completed.\n";
}


void Test::run_path_integrity_test(const std::string& fen, int num_moves){
    Board board(fen);
    Board original_board(fen);

    std::vector<Move> moves_made;

    for(int i=0; i < num_moves; ++i){
        MoveSaver moves(board, LEGAL);
        if(moves.is_empty()){
            break; // no more legal moves
        }
        
        size_t move_index = i % moves.size();
        Move m = moves[move_index];
        board.do_move(m,true);
        moves_made.push_back(m);
    }

    //undo moves
    for(int i = moves_made.size() -1; i >=0; --i){
        board.undo_move();
        board.sanity_check();
    }

    if(board.get_board_info() != original_board.get_board_info()){
        std::cout << "Random path integrity test failed for FEN: " << fen << "\n";
        std::cout << "After making and undoing " << num_moves << " moves.\n";
        std::cout << "Expected: \n" << original_board.get_board_info() << "\n";
        std::cout << "Got: \n" << board.get_board_info() << "\n";

        std::cout << "Moves made:\n";
        for(const Move& m : moves_made){
            std::cout << m.to_str() << " ";
        }
        std::cout << "\n";
    }
}

void Test::zobrist_suite(){
    std::cout << "Running zobrist suite...\n";
    int max_moves = 30;
    int max_tests_per_fen = 3000;
    for(const std::string& fen : fens){
        for(int i = 0; i < max_tests_per_fen; ++i)
            run_zobrist_test_random(fen,max_moves);

        std::cout << "ok" << std::endl;
    }
    std::cout << "Zobrist suite completed.\n";
}

void Test::run_zobrist_test_random(const std::string& fen, int num_moves){
    Board board(fen);
    Key initial_key = board.get_key();

    std::vector<Key> keys;
    std::vector<Move> moves_made;

    keys.push_back(initial_key);

    for(int i=0; i < num_moves; ++i){
        MoveSaver moves(board, LEGAL);
        if(moves.is_empty()){
            break; // no more legal moves
        }
        size_t move_index = rand() % moves.size();
        Move m = moves[move_index];
        board.do_move(m,true);

        moves_made.push_back(m);
        keys.push_back(board.get_key());

        if(board.get_key() != board.gen_zobrist_key() ){
            std::cout << "Zobrist test failed for FEN: " << fen << "\n";
            std::cout << "After move: " << m.to_str() << "\n";  
            std::cout << "Expected key: " << board.gen_zobrist_key() << "\n";
            std::cout << "Got key: " << board.get_key() << "\n";

            std::cout << "Moves made:\n";
            for(const Move& mv : moves_made){
                std::cout << mv.to_str() << " ";
            }
            std::cout << "\n";
            return;
        }
    }

    //undo moves
    for(int i = moves_made.size() -1; i >=0; --i){
        board.undo_move();
        if(board.get_key() != keys[i]){
            std::cout << "Zobrist test failed during undo for FEN: " << fen << "\n";
            std::cout << "After undoing move: " << moves_made[i].to_str() << "\n";  
            std::cout << "Expected key: " << keys[i] << "\n";
            std::cout << "Got key: " << board.get_key() << "\n"; 
            std::cout << "Moves made:\n";
            for(const Move& mv : moves_made){
                std::cout << mv.to_str() << " ";
            }
            std::cout << "\n";
            return;
        }
    }

}

void Test::run_all(){
    std::cout << "Starting test suites...\n";
    integrity_suite();
    random_path_integrity_test();
    zobrist_suite();
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
    Board original_board(fen);

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
    if (depth == 1 ) return moves.size();
    for(const Move& m : moves){
        board.do_move(m,true);
        nodes += perft(board, depth - 1);
        board.undo_move();
    }
    return nodes;
}


void Test::full_challenge(){
    std::cout << "Running full challenge test...\n";

    Board boards[2];
    for(auto& fen : fens){
        int turn = 0;

        boards[0] = Board(fen); //White
        boards[1] = Board(fen); //Black

        while(true){
            Board& board = boards[turn %2];
            Board& opponent_board = boards[(turn+1)%2];

        

            Result result = search(board, 6); //2 seconds per move

            if(result.best_move == Move::null()){
                std::cout << "Game over on turn " << turn << "\n";
                break;
            }

            board.do_move(result.best_move,true);
            opponent_board.do_move(result.best_move,true);

            std::cout << (turn %2 ==0 ? "White" : "Black") << " plays: " << result.best_move.to_str() << "\n";
            turn++;

            if(turn > 2000){
                std::cout << "Draw by move limit.\n";
                break;
            }

        }
    }




    std::cout << "Full challenge test completed.\n";
}
} // namespace BiggerBotChess