#pragma once
#include "board.h"
#include "movegenerator.h"

namespace BiggerBotChess {

class UCIEngine{
public:
    UCIEngine();
    void main_loop();

private:

    void handle_position(const std::string& command);
    void handle_go(const std::string& command);
    void handle_newgame();
    Board m_Board;
};


} // namespace BiggerBotChess