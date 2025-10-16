#include <iostream>
#include "board.h"
#include "move.h"
#include "movegenerator.h"
#include "uci.h"
#include <algorithm>
#include <chrono>
using namespace BiggerBotChess;

int perft(Board& board, int depth) {
    if (depth == 0) return 1;

    MoveSaver moves(board, ALL);
    if (moves.is_empty()) return 0;   

    int nodes = 0;
    for (const Move& move : moves) {
        
        if(!board.do_move(move,false)){
            continue;
        }

        int now = perft(board, depth - 1);
        nodes += now;
        if(depth == 10){
            std::cout << move.to_str() << " " << now <<"\n";
        }
       
        board.undo_move();
    }
    return nodes;
}


int main() {
    

    std::cout << "Hello! I am BiggerBotChess!" << std::endl;
    

    Board::init();
    BB::init();

    Board b;
    std::cout <<"started perft test\n";
    std::cin.get();
    //time it 
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();
    std::cout << perft(b, 7) << std::endl;
    auto t2 = high_resolution_clock::now();

     //Getting number of milliseconds as an integer. 
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

     //Getting number of milliseconds as a double. 
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << ms_int.count() << "ms\n";
    std::cout << ms_double.count() << "ms\n";
    
    std::cout <<"finished perft test\n";
/*
    UCIEngine engine;
    engine.main_loop();*/


    //Perft test
    
    /*
    Board board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ");
    
    std::string save_info = board.get_board_info();
    std::string save_pretty =  board.get_board_pretty();
    std::string save_pretty_bb =  board.get_board_pretty_bb();

    std::cout<<board.m_StateHistoryIndex<<"\n";

    MoveSaver moves(board, LEGAL);
    for(const Move& m : moves){
       // std::cout << "Doing move: " << m.to_str() << "\n";
        board.do_move(m,false);
        board.sanity_check();
        MoveSaver moves2(board, LEGAL);
        std::string now_info = board.get_board_info();
        std::string now_pretty =  board.get_board_pretty();
        std::string now_pretty_bb =  board.get_board_pretty_bb();
        for(const Move& m2 : moves2){
            //std::cout << "len moves : " << board.m_MoveList.size() << std::endl;
            
           // std::cout << "Doing move: "<< m.to_str() << " && " << m2.to_str() << "\n";
            board.do_move(m2,false);
            board.sanity_check();
            MoveSaver moves3(board, LEGAL);
            for(const Move& m3 : moves3){
                std::cout << "Doing move: " << m.to_str() << " && "<< m2.to_str() << " && "<<m3.to_str() << "\n";
                board.do_move(m3,false);
                
                //std::cout << board.get_board_info() << "\n";
                board.sanity_check();
                board.undo_move();
                
                board.sanity_check();
                board.undo_move();
                board.sanity_check();
                board.undo_move();
                board.sanity_check();
                now_info = board.get_board_info();
                now_pretty =  board.get_board_pretty();
                now_pretty_bb =  board.get_board_pretty_bb();
            
                if(now_info != save_info || now_pretty != save_pretty || now_pretty_bb != save_pretty_bb){
                    std::cout << "Error after move " << m.to_str() << " && "<< m2.to_str()<< "&&"<<m3.to_str()<< ":\n";
                    std::cout << "Before:\n" << save_info << "\n" << save_pretty << "\n" << save_pretty_bb << "\n";
                    std::cout << "After:\n" << now_info << "\n" << now_pretty << "\n" << now_pretty_bb << "\n";
                    return -1;
                }
                board.do_move(m,false);
                board.do_move(m2,false);
            } 
            board.undo_move();
        
        }

        board.undo_move();
        
        if(now_info != save_info || now_pretty != save_pretty || now_pretty_bb != save_pretty_bb){
            std::cout << "Error after move " << m.to_str() << ":\n";
            std::cout << "Before:\n" << save_info << "\n" << save_pretty << "\n" << save_pretty_bb << "\n";
            std::cout << "After:\n" << now_info << "\n" << now_pretty << "\n" << now_pretty_bb << "\n";
            return -1;
        }
        
    }
   


    int depth = 7;
    std::cout << "Starting perft test from position:\n" << board.get_board_pretty() << "\n";
    int result = perft(board, depth);
    std::cout << "Perft to depth " << depth << ": " <<  result << std::endl;
*/

    return 0;
}
