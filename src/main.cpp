#include <iostream>
#include "board.h"
#include "move.h"
#include "movegenerator.h"

int main() {
    using namespace BiggerBotChess;

    std::cout << "Hello, BiggerBotChess!" << std::endl;

    Board::init();

    Board board("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
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



    return 0;
}
