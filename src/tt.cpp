#include "tt.h"



namespace BiggerBotChess {



void TT::init(){
    m_table.resize(TABLE_SIZE, TTEntry{0, EXACT, Move::null(), 0, 0});
}

void TT::clear(){
    m_table.clear();
    init();
}

TTResult TT::probe(Key key){
    size_t index = key % TABLE_SIZE;
    TTEntry& entry = m_table[index];
    if(entry.key == key){
        return {true, entry.flag, entry.best_move, entry.score, entry.depth};
    } else {
        return {false, EXACT, Move::null(), 0, 0};
    }
}

void TT::store(Key key, TTFlag flag, Move best_move, Eval::Evaluation score, int depth){
    size_t index = key % TABLE_SIZE;
    TTEntry& entry = m_table[index];
    //Replace only if the new entry has greater or equal depth
    if(entry.key != key || depth >= entry.depth){
        entry.key = key;
        entry.flag = flag;
        entry.best_move = best_move;
        entry.score = score;
        entry.depth = depth;
    }
}

} // namespace BiggerBotChess


