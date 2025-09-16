#include "core/engine.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "BiggerBot Chess Engine v1.0\n";
    std::cout << "============================\n\n";
    
    // Initialize the chess engine
    chess::Engine engine;
    engine.initialize();
    
    std::cout << "Initial position:\n";
    std::cout << engine.getCurrentPosition() << "\n\n";
    
    // Simple interactive loop
    std::string command;
    std::cout << "Commands:\n";
    std::cout << "  move <move>  - Make a move (e.g., 'move e2e4')\n";
    std::cout << "  search       - Find best move\n";
    std::cout << "  position     - Show current position (FEN)\n";
    std::cout << "  quit         - Exit the program\n\n";
    
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);
        
        if (command == "quit" || command == "exit") {
            break;
        } else if (command == "position") {
            std::cout << "Current position: " << engine.getCurrentPosition() << "\n";
        } else if (command == "search") {
            std::cout << "Searching for best move...\n";
            std::string bestMove = engine.findBestMove(4);
            std::cout << "Best move: " << bestMove << "\n";
        } else if (command.substr(0, 4) == "move") {
            if (command.length() > 5) {
                std::string move = command.substr(5);
                if (engine.makeMove(move)) {
                    std::cout << "Move " << move << " played.\n";
                    if (engine.isGameOver()) {
                        std::cout << "Game over!\n";
                    }
                } else {
                    std::cout << "Invalid move: " << move << "\n";
                }
            } else {
                std::cout << "Usage: move <move> (e.g., move e2e4)\n";
            }
        } else if (!command.empty()) {
            std::cout << "Unknown command: " << command << "\n";
        }
    }
    
    std::cout << "Goodbye!\n";
    return 0;
}