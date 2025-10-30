#include "uci.h"
#include "search.h"
#include <sstream>
#include <iostream>
#include <fstream>

namespace BiggerBotChess {

class MyOut{
public:
    std::ofstream log_file;
    MyOut(){
        //log_file.open("/home/barb/Desktop/Coding_progejcts/BiggerBotChess/out.lol", std::ios::app);
    }
    ~MyOut(){
        //log_file.close();
    }

    void only_file(const std::string& data){
        //log_file << data;
        //log_file.flush();
    }

    template<typename T>
    MyOut& operator<<(const T& data){
        //log_file << data;
        std::cout << data;
        //log_file.flush();
        std::cout.flush();
        return *this;
    }
};
MyOut log_file = MyOut();

UCIEngine::UCIEngine() : m_Board() {
}

void UCIEngine::main_loop() {
    std::string command;
    // For debugging purposes, log all commands to a file
   

    while (true) {
        std::getline(std::cin, command);
        if(command.empty()) continue;

        log_file.only_file(command+"\n");

        if (command == "uci") {
            log_file << "id name BiggerBotChess 0.1\n";
            log_file << "id author biggerbomber\n\n";
            //log_file << "option name Threads type spin default 1 min 1 max 1024\n";
            log_file << "uciok\n";
        } else if (command == "isready") {
            log_file << "readyok\n";
        }else if(command == "ucinewgame"){
           handle_newgame();
        } else if (command.substr(0, 2) == "go") {
            handle_go(command);
        } else if (command.substr(0, 8) == "position") {
            handle_position(command);
        } else if (command == "quit") {
            break;
        } else if(command == "d"){
            log_file << m_Board.get_board_pretty_bb() << "\n";
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
    
    if(command.substr(0, 2) != "go") return;
    
    std::stringstream ss(command);
    std::string token;
    ss >> token; // skip "go"   
    ss >> token;
    if(token == "depth"){
        int depth;
        ss >> depth;
        Result best = search(m_Board, depth);
        log_file << "info score cp "<<best.score<<" depth "<<depth<< " pv "<<best.best_move.to_str()<<"\n";
        log_file << "bestmove " << best.best_move.to_str() << "\n";
        return;
    }
    Result best = search(m_Board, 6); //search to depth 6
    log_file << "info score cp "<<best.score<<" depth 6 pv "<<best.best_move.to_str()<<"\n";
    log_file << "bestmove " << best.best_move.to_str() << "\n";
}
} // namespace BiggerBotChess