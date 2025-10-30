#include "search.h"
#include "movegenerator.h"

namespace BiggerBotChess {

using namespace Eval;

Result q_search(Board& board, int alpha, int beta){
    //Color to_move = board.get_color();

    if(board.is_draw()){
        return {Move::null(), 0};
    }
    MoveSaver moves(board, ALL);
    

    moves.m_free_pos = moves.m_CaptureSize; //only search captures in quiescence

    if(moves.is_empty()){
        return {Move::null(), eval(board)};
    }

    Evaluation stand_pat = eval(board);
    if(stand_pat >= beta){
        return {Move::null(), stand_pat};
    }
    if(stand_pat > alpha){
        alpha = stand_pat;
    }
    bool one_move_found = false;
    Result best_result = {Move::null(), alpha};
    for(const Move& m : moves){
        if(!board.do_move(m,false)){
            continue;
        }
        one_move_found = true;
        
        Result r = q_search(board, -beta, -alpha);
        r.score = -r.score;
        
        board.undo_move();
        

        if(r.score > best_result.score || best_result.best_move == Move::null()){
            best_result.best_move = m;
            best_result.score = r.score;
            alpha = std::max(alpha, r.score);
        }

        if(alpha >= beta){
            break; //beta cutoff
        }
    }

    if(!one_move_found){
       if(board.is_in_check(board.get_color())){
            return {Move::null(), -mate_in_ply(0)};
        } else {
            return {Move::null(), 0}; //stalemate
        }
    }
    return best_result;
}

Result i_search(Board& board, int depth, int alpha, int beta){
    //Color to_move = board.get_color();
    if(board.is_draw()){
        return {Move::null(), 0};
    }
    if(depth == 0){
        return q_search(board, alpha, beta);
    }

    TTResult tt_result = board.m_tt.probe(board.get_key());
    
    Move tt_move = tt_result.found ? tt_result.best_move : Move::null();
    
    MoveSaver moves(board, ALL, tt_move);
    
    bool one_move_found = false;
    
    Result best_result = {Move::null(), alpha};
    for(const Move& m : moves){
        if(!board.do_move(m,false)){
            continue;
        }
        one_move_found = true;
        
        Result r = i_search(board, depth-1, -beta, -alpha);
        r.score = -r.score;
       
        board.undo_move();
        


        if(r.score > best_result.score || best_result.best_move == Move::null()){
            best_result.best_move = m;
            best_result.score = r.score;
        }
        alpha = std::max(alpha, r.score);
        if(alpha >= beta){
            break; //beta cutoff
        }
    }
    if(!one_move_found){
        if(board.is_in_check(board.get_color())){
            return {Move::null(), -mate_in_ply(0)};
        } else {
            return {Move::null(), 0}; //stalemate
        }
    }
    board.m_tt.store(board.get_key(), TTFlag::EXACT, best_result.best_move, best_result.score, depth);

    return best_result;

}


Result search(Board& board, int depth){
    return i_search(board, depth, -100000, 100000);
}

} // namespace BiggerBotChess