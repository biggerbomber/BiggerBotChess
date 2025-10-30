

#include "move.h"
#include "board.h"
#include "eval.h"
#include "tt.h"

namespace BiggerBotChess {
using namespace Eval;
struct Result{
    Move best_move = Move::null();
    Evaluation score = 0;
};


Result search(Board& board, int depth);

} // namespace BiggerBotChess