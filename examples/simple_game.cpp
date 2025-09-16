#include "core/engine.h"
#include <iostream>

int main() {
    std::cout << "Simple Chess Game Example\n";
    std::cout << "========================\n";
    
    chess::Engine engine;
    engine.initialize();
    
    std::cout << "Initial position: " << engine.getCurrentPosition() << "\n";
    
    // Make a few moves
    std::cout << "\nMaking moves: e2e4, e7e5\n";
    
    if (engine.makeMove("e2e4")) {
        std::cout << "White played e2e4\n";
        std::cout << "Position: " << engine.getCurrentPosition() << "\n";
    }
    
    if (engine.makeMove("e7e5")) {
        std::cout << "Black played e7e5\n";
        std::cout << "Position: " << engine.getCurrentPosition() << "\n";
    }
    
    // Find best move
    std::cout << "\nSearching for best move...\n";
    std::string bestMove = engine.findBestMove(3);
    std::cout << "Engine suggests: " << bestMove << "\n";
    
    return 0;
}