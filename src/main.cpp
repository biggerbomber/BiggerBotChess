#include <iostream>
#include "board.h"
#include "move.h"
#include "movegenerator.h"

int main() {
    using namespace BiggerBotChess;

    std::cout << "Hello, BiggerBotChess!" << std::endl;

    Board::init();

    Board board("r3k2r/p1p1ppb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 3 2");
    std::cout << board.get_board_info() << std::endl;
    std::cout << board.get_board_pretty_bb() << std::endl;
    std::cout << board.get_board_pretty() << std::endl;

    std::cout << sizeof(Board) << std::endl;

        /*/
    BitBoard b = 0;
    BitBoard mask = get_mask(S_A1) | get_mask(S_A2) | get_mask(S_A3);
    do
        {
            b = (b - mask) & mask;
            std::cout << print_bitboard(b)<< std::endl;
        } while (b);
    
    */


    BB::init();
    
    //testing

    for(Square sq = S_FIRST; sq <= S_LAST; ++sq){
       //compare plain attacks with magic attacks and print errors
        //BISHOP
        BitBoard plain = BB::s_PlainAttacks[BISHOP][sq];
        BitBoard magic = BB::get_attacks(sq, BISHOP);
        if(plain != magic){
            std::cout << "Error in BISHOP attacks on square " << square_to_str(sq) << "\n";
            std::cout << "Plain:\n" << print_bitboard(plain) << "\n";
            std::cout << "Magic:\n" << print_bitboard(magic) << "\n";
        }
        //ROOK
        plain = BB::s_PlainAttacks[ROOK][sq];
        magic = BB::get_attacks(sq, ROOK);
        if(plain != magic){
            std::cout << "Error in ROOK attacks on square " << square_to_str(sq) << "\n";
            std::cout << "Plain:\n" << print_bitboard(plain) << "\n";
            std::cout << "Magic:\n" << print_bitboard(magic) << "\n";
        }
        //QUEEN
        plain = BB::s_PlainAttacks[QUEEN][sq];
        magic = BB::get_attacks(sq, QUEEN);
        if(plain != magic){
            std::cout << "Error in QUEEN attacks on square " << square_to_str(sq) << "\n";
           // std::cout << "Plain:\n" << print_bitboard(plain) << "\n";
            //std::cout << "Magic:\n" << print_bitboard(magic) << "\n";
        }

    }

    BitBoard occupancy = board.get_pieces(WHITE,ALL_PIECES) | board.get_pieces(BLACK,ALL_PIECES);

    std::cout << "Occupancy:\n" << print_bitboard(occupancy) << "\n";
    //queen e2
    std::cout << "Queen e2 attacks:\n" << print_bitboard(BB::get_attacks(S_C4,BISHOP,occupancy) & ( board.get_pieces(BLACK,ALL_PIECES) | ~occupancy)) << "\n";
    //rook a1
    std::cout << "Rook a1 attacks:\n" << print_bitboard(BB::get_attacks(S_A1,ROOK,occupancy) & ( board.get_pieces(BLACK,ALL_PIECES) | ~occupancy)) << "\n";



    //test is_square_attacked in board, print a borad with all attacked squares marked with X
    
    std::string board_str;
    board_str += "   ";
    for(File f = FILE_A; f <= FILE_H; ++f) {
        board_str += "| "+ std::string(1, 'a' + static_cast<char>(f)) + " ";
    }
    board_str += "| \n";
    board_str += "---+---+---+---+---+---+---+---+---+\n";      
    for(Rank r = RANK_8; r != RANK_OVERFLOW; --r) {
        board_str += " " + std::string(1, '1' + static_cast<char>(r)) + " ";
        for(File f = FILE_A; f <= FILE_H; ++f) {
            Square sq = get_square(r, f);
            if(board.is_square_attacked(sq, WHITE)) {
                board_str += "| X ";
            } else {
                board_str += "|   ";
            }
        }
        board_str += "|\n";
        board_str += "---+---+---+---+---+---+---+---+---+\n";
    }
    std::cout << "Squares attacked by BLACK:\n" << board_str << "\n";
    


    //test castling
    //board = Board("r3k2r/PPppp1PP/8/8/8/8/pppPPPpp/R3K2R w KQkq - 0 1");
    std::cout << board.get_board_pretty_bb() << std::endl;
    std::cout << "White can castle king side: " << board.is_castle_possible(WHITE, KING_SIDE) << "\n";
    std::cout << "White can castle queen side: " << board.is_castle_possible(WHITE, QUEEN_SIDE) << "\n";
    std::cout << "Black can castle king side: " << board.is_castle_possible(BLACK, KING_SIDE) << "\n";
    std::cout << "Black can castle queen side: " << board.is_castle_possible(BLACK, QUEEN_SIDE) << "\n";


    MoveSaver moves(board, CAPTURE);

    std::cout << "# of moves: " << moves.size() << "\n"; 
    for(const Move& m : moves){
        std::cout << m.to_str() << "\n";
    }

    return 0;
}
