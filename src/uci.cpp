#include "uci.h"
#include <sstream>
#include <iostream>
#include <fstream>

namespace BiggerBotChess {


UCIEngine::UCIEngine() : m_Board() {
    
}

void UCIEngine::main_loop() {
    std::string command;
    // For debugging purposes, log all commands to a file
    std::ofstream log_file;
    log_file.open("/home/barb/Desktop/Coding_progejcts/BiggerBotChess/out.lol", std::ios::app);
    log_file << "New session\n";

    while (true) {
        std::getline(std::cin, command);
        if(command.empty()) continue;

        log_file << command << std::endl;
        log_file.flush();

        if (command == "uci") {
            std::cout << "id name BiggerBotChess 0.1\n";
            std::cout << "id author biggerbomber\n\n";
            //std::cout << "option name Threads type spin default 1 min 1 max 1024\n";
            std::cout << "uciok\n";
        } else if (command == "isready") {
            std::cout << "readyok\n";
        }else if(command == "ucinewgame"){
           handle_newgame();
        } else if (command.substr(0, 2) == "go") {
            handle_go(command);
        } else if (command.substr(0, 8) == "position") {
            handle_position(command);
        } else if (command == "quit") {
            break;
        } else if(command == "d"){
            std::cout << m_Board.get_board_pretty_bb() << std::endl;
        } 
        
    }
}


void UCIEngine::handle_newgame() {
    m_Board = Board();
}

void UCIEngine::handle_position(const std::string& command) {
    std::stringstream ss(command);
    std::string token;
    std::string fen;

    ss >> token;
    if(token != "position") return;
    ss >> fen;
    if(fen == "startpos") {
        m_Board = Board();
    } else if(fen == "fen") {
        std::string fen_str;
        while (ss >> token && token != "moves")
        {
            fen_str += token + " ";
        }
        fen_str.pop_back(); // remove last space
    
        m_Board = Board(fen_str);
    }
    while(ss >> token) {
        Move m = m_Board.str_to_move(token);
        if(m == Move::null()) {
            continue;
        }
        m_Board.do_move(m,false);
    }

}

void UCIEngine::handle_go(const std::string& command) {
    // For simplicity, we will just make a random legal move
    if(command.substr(0, 2) != "go") return;
    MoveSaver moves(m_Board, LEGAL);
    if (moves.is_empty()) {
        std::cout << "bestmove (none)\n";
        return;
    }
    const Move& best_move = *moves.begin(); // Just pick the first legal move
    std::cout << "info score cp 0 depth 1 nodes " << moves.size() << " nps " << moves.size() << "\n";
    std::cout << "bestmove " << best_move.to_str() << "\n";
}
} // namespace BiggerBotChess