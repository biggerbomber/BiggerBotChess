#include "search.h"
#include "move.h"
#include "movegenerator.h"
#include "timemanager.h"
#include <cassert>
#include <cstring>
#include <string>
namespace BiggerBotChess {

using namespace Eval;

Move pvTable[MAX_DEPTH][MAX_DEPTH];
int currentDepth;




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

Result i_search(Board& board, int depth, int alpha, int beta, Timemanager& tm){
    //Color to_move = board.get_color();
    if(board.is_draw()){
        return {Move::null(), 0};
    }
    if(depth == 0){
        return q_search(board, alpha, beta);
    }
    if(tm.time_to_stop()){
        return {Move::null(), 0};
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

        Result r = i_search(board, depth-1, -beta, -alpha, tm);
        r.score = -r.score;
       
        board.undo_move();
        


        if(r.score > best_result.score || best_result.best_move == Move::null()){
            best_result.best_move = m;
            best_result.score = r.score;

            //copiare la pv table inferiore su

            pvTable[currentDepth-depth][currentDepth-depth] = m;
            for(int i = currentDepth-depth+1; i <currentDepth; i++){
                pvTable[currentDepth-depth][i] = pvTable[currentDepth-depth+1][i];
            }
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

std::string print_pvTable(unsigned int depth){
    assert(depth <= MAX_DEPTH);
    std::string out;
    for(unsigned int i = 0; i<depth;i++){
        for(unsigned int j = 0; j<depth;j++){
            out += pvTable[i][j].to_str() + " ";
        }
        out += "\n";
    }
    return out;
}


Result search(Board& board, int depth, Timemanager& tm){

    memset(pvTable,0,sizeof(pvTable));
    //Iterative deepening
    Result old;

    Evaluation bound = Eval::eval(board);
    int delta = 50; //initial aspiration window size


    for(int d = 1; d<=depth;++d){
        currentDepth = d;
        Result r;
        int alpha = bound - delta;
        int beta  = bound + delta;
        bool end = false;
        int mult = 1;
        do {
            if(tm.time_to_stop()){
                return old;
            }
            r = i_search(board, d, alpha, beta, tm);

            if(r.score<=alpha){
                //std::cout << "FAILED LOW at depth "<<d<<" score "<<r.score<<" alpha "<<alpha<<std::endl;
                alpha -=delta*mult;
            }if(r.score>beta){
                //std::cout << "FAILED HIGH at depth "<<d<<" score "<<r.score<<" beta "<<beta<<std::endl;

                beta += delta*mult;
            }else{
                end = true;
            }
            mult++;
        }while(!end);
        std::string pv_string ="";
        for(int i=0; i<d;i++){
            pv_string += pvTable[0][i].to_str()+" ";
        }
        std::cout << "info depth "<<d<<" score cp "<<r.score<<" pv "<<pv_string<<std::endl;
        //std::cout << print_pvTable(d*3) <<std::endl;
        if(d == depth){
            return r;
        }
        old = r;
    }

    return old;
}
} // namespace BiggerBotChess