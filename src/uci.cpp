#include "uci.h"
#include "search.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include "timemanager.h"

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

    void only_file([[maybe_unused]] const std::string&  data){
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

bool  UCIEngine::is_first_move() const {
    return m_Board.m_StateHistory.size() < 3; 
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
        } else  if(command == "test"){
            handle_testing();
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
    //parameter of go : depth <x>, wtime <x> btime <x> winc <x> binc <x>

    int wtime = 100000, btime = 100000, winc = 0, binc = 0, depth = MAX_DEPTH;
    std::stringstream ss(command);
    std::string token;
    ss >> token; // skip "go"
    while(ss >> token) {
        if(token == "wtime") {
            ss >> wtime;
        } else if(token == "btime") {
            ss >> btime;
        } else if(token == "winc") {
            ss >> winc;
        } else if(token == "binc") {
            ss >> binc;
        }
    }

    if(is_first_move()){
        wtime = std::min(wtime,50000);
        winc  = std::min(winc,1000);
        btime = std::min(btime,50000);
        binc  = std::min(binc,1000);
    }

    Timemanager tm(wtime, btime, winc, binc, m_Board.get_color());
    tm.start_timer();

    Result result = search(m_Board, depth, tm);
    log_file << "bestmove " << result.best_move.to_str() << "\n";

}

void UCIEngine::handle_testing(){
    std::string line;
    while (std::getline(std::cin, line)) {
        if(line == "quit"){
            break;
        }
        Board board(line);
        Features feats = Eval::get_features(board);
        //log_file << "Features for position: " << line << "\n";
        log_file << features_to_str_compact(feats) << "\n";
    }
}

} // namespace BiggerBotChess