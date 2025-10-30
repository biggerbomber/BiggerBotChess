#include <iostream>
#include "board.h"
#include "move.h"
#include "movegenerator.h"
#include "uci.h"
#include <algorithm>
#include <chrono>
#include "test.h"
#include "eval.h"

#include "tt.h"
using namespace BiggerBotChess;


int main() {
    

    //std::cout << "Hello! I am BiggerBotChess!" << std::endl;
    

    Board::init();
    BB::init();
    Eval::init();
    
    //Test::full_challenge();
    //Test::run_all();


    UCIEngine engine;
    engine.main_loop();

    return 0;
}
