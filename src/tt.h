#pragma once
#include "types.h"
#include "move.h"
#include "eval.h"
#include <vector>

namespace BiggerBotChess {
enum TTFlag : int8_t {
    EXACT,
    LOWERBOUND,
    UPPERBOUND
};
struct TTResult
{
    bool found;
    TTFlag flag;
    Move best_move;
    Eval::Evaluation score;
    int depth;
};

struct TTEntry {
    Key key;
    TTFlag flag;
    Move best_move;
    Eval::Evaluation score;
    int depth;
};



class TT
{
public:
    static constexpr size_t TABLE_SIZE = 1 << 26;
    void init();
    void clear();

    TTResult probe(Key key);
    void store(Key key, TTFlag flag, Move best_move, Eval::Evaluation score, int depth);

    std::vector<TTEntry> m_table;
};


} // namespace BiggerBotChess