#include "search.h"
#include "movegenerator.h"

namespace BiggerBotChess {

int simple_eval(Board& board){
    int score = 0;
    //piece values
    const int piece_values[PIECE_NUM] = {0,100,320,330,500,900,0};
    for(Square s = S_FIRST; s<=S_LAST;++s){
        Piece p = board.get_piece_on(s);
        if(p != NONE_PIECE){
            Color c = p >> BLACK_PIECE_OFFSET ? BLACK : WHITE;
            PieceType pt = get_piece_type(p);
            score += (c == WHITE) ? piece_values[pt] : -piece_values[pt];
        }
    }
    return score;
}

Result q_search(Board& board, int alpha, int beta){
    //Color to_move = board.get_color();

    MoveSaver moves(board, CAPTURE);
    if(moves.is_empty()){
        return {Move::null(), simple_eval(board)};
    }

    Result best_result;
    for(const Move& m : moves){
        if(!board.do_move(m,false)){
            continue;
        }
        Result r = q_search(board, -beta, -alpha);
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
    return best_result;
}

Result i_search(Board& board, int depth, int alpha, int beta){
    //Color to_move = board.get_color();
    if(depth == 0){
        return q_search(board, alpha, beta);
    }

    MoveSaver moves(board, ALL);
    if(moves.is_empty()){
        return {Move::null(), -10000};
    }

    Result best_result;
    for(const Move& m : moves){
        if(!board.do_move(m,false)){
            continue;
        }
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
    return best_result;

}


Result search(Board& board, int depth){
    return i_search(board, depth, -100000, 100000);
}

} // namespace BiggerBotChess