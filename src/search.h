

#include "move.h"
#include "board.h"
#include "eval.h"
#include "tt.h"
namespace BiggerBotChess {

constexpr int MAX_DEPTH = 64;
class Timemanager;
using namespace Eval;
struct Result{
    Move best_move = Move::null();
    Evaluation score = 0;
};


Result search(Board& board, int depth, Timemanager& tm);

} // namespace BiggerBotChess