

#include "move.h"
#include "board.h"

namespace BiggerBotChess {
struct Result{
    Move best_move = Move::null();
    int score = 0;
};


Result search(Board& board, int depth);

} // namespace BiggerBotChess