#include <iostream>
#include "board.h"
#include "move.h"
#include "movegenerator.h"
#include <algorithm>
using namespace BiggerBotChess;

int perft(Board& board, int depth) {
    if (depth == 0) return 1;

    MoveSaver moves(board, LEGAL);
    if (moves.is_empty()) return 0;   

    int nodes = 0;
    for (const Move& move : moves) {
        board.do_move(move,true);

        int now = perft(board, depth - 1);
        nodes += now;
       
        board.undo_move();
    }
    return nodes;
}


int main() {
    

    std::cout << "Hello, BiggerBotChess!" << std::endl;

    Board::init();
    BB::init();

    //Perft test
    
    
    /*
    
    std::string save_info = board.get_board_info();
    std::string save_pretty =  board.get_board_pretty();
    std::string save_pretty_bb =  board.get_board_pretty_bb();

    MoveSaver moves(board, LEGAL);
    for(const Move& m : moves){
        board.do_move(m);
        MoveSaver moves2(board, LEGAL);
        std::string now_info = board.get_board_info();
        std::string now_pretty =  board.get_board_pretty();
        std::string now_pretty_bb =  board.get_board_pretty_bb();
        for(const Move& m2 : moves2){
            //std::cout << "len moves : " << board.m_MoveList.size() << std::endl;
            
            //std::cout << board.get_board_pretty() << "\n";
            board.do_move(m2);
            MoveSaver moves3(board, LEGAL);
            for(const Move& m3 : moves3){
                //std::cout << "Doing move: " << m.to_str() << " && "<< m2.to_str() << " && "<<m3.to_str() << "\n";
                board.do_move(m3);
                board.undo_move();
                board.undo_move();
                board.undo_move();
                now_info = board.get_board_info();
                now_pretty =  board.get_board_pretty();
                now_pretty_bb =  board.get_board_pretty_bb();
            
                if(now_info != save_info || now_pretty != save_pretty || now_pretty_bb != save_pretty_bb){
                    std::cout << "Error after move " << m.to_str() << " && "<< m2.to_str()<< "&&"<<m3.to_str()<< ":\n";
                    std::cout << "Before:\n" << save_info << "\n" << save_pretty << "\n" << save_pretty_bb << "\n";
                    std::cout << "After:\n" << now_info << "\n" << now_pretty << "\n" << now_pretty_bb << "\n";
                    return -1;
                }
                board.do_move(m);
                board.do_move(m2);
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
        
    }*/
   
    Board board("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

    int depth = 6;
    std::cout << "Starting perft test from position:\n" << board.get_board_pretty() << "\n";
    int result = perft(board, depth);
    std::cout << "Perft to depth " << depth << ": " <<  result << std::endl;


    return 0;
}
