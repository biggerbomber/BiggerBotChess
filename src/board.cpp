#include "board.h"
#include <cassert>
#include <cstring>

namespace BiggerBotChess {


void Board::init() {
   //Init
}

BitBoard rank_file_to_square_bb(Rank r, File f) {
    return (1ULL << (r * (uint8_t)8 + f));
}

Square rank_file_to_square(Rank r, File f) {
    return static_cast<Square>(r * (uint8_t)8 + f);
}

Rank square_to_rank(Square sq) {
    return static_cast<Rank>(((uint8_t)sq / (uint8_t)8));
}

File square_to_file(Square sq) {
    return static_cast<File>(((uint8_t)sq % (uint8_t)8));
}


Board::Board(   const std::string& fen,
                const std::string& moves) {
    assert(moves == ""); // moves not implemented yet
    clear();

    int pos_end = fen.find(' ',0);

    File f = FILE_A;
    Rank r = RANK_8;
    for (int i=0;i<pos_end;i++) {
        char c  = fen[i];

        if (c == '/') {
            r = static_cast<Rank>(r - 1);
            f = FILE_A;
            continue;
        }
        Square sq = rank_file_to_square(r, f);
        ++f;

        switch (c) {
            case 'P': m_Pawns[WHITE] |= (1ULL << sq); m_Board[sq] = W_PAWN; break;
            case 'p': m_Pawns[BLACK] |= (1ULL << sq); m_Board[sq] = B_PAWN; break;
            case 'N': m_Knights[WHITE] |= (1ULL << sq); m_Board[sq] = W_KNIGHT; break;
            case 'n': m_Knights[BLACK] |= (1ULL << sq); m_Board[sq] = B_KNIGHT; break;
            case 'B': m_Bishops[WHITE] |= (1ULL << sq); m_Board[sq] = W_BISHOP; break;
            case 'b': m_Bishops[BLACK] |= (1ULL << sq); m_Board[sq] = B_BISHOP; break;
            case 'R': m_Rooks[WHITE] |= (1ULL << sq); m_Board[sq] = W_ROOK; break;
            case 'r': m_Rooks[BLACK] |= (1ULL << sq); m_Board[sq] = B_ROOK; break;
            case 'Q': m_Queens[WHITE] |= (1ULL << sq); m_Board[sq] = W_QUEEN; break;
            case 'q': m_Queens[BLACK] |= (1ULL << sq); m_Board[sq] = B_QUEEN; break;
            case 'K': m_Kings[WHITE] |= (1ULL << sq); m_Board[sq] = W_KING; break;
            case 'k': m_Kings[BLACK] |= (1ULL << sq); m_Board[sq] = B_KING; break;
            default:

                if (isdigit(c)) {
                    f = static_cast<File>(f + (c - '0')-1);
                }
                break;
        }
    }
    m_Occupancy = m_Pawns[WHITE] | m_Pawns[BLACK] |
                  m_Knights[WHITE] | m_Knights[BLACK] |
                  m_Bishops[WHITE] | m_Bishops[BLACK] |
                  m_Rooks[WHITE] | m_Rooks[BLACK] |
                  m_Queens[WHITE] | m_Queens[BLACK] |
                  m_Kings[WHITE] | m_Kings[BLACK];

    m_Pieces[WHITE] = m_Pawns[WHITE] |
                      m_Knights[WHITE] |
                      m_Bishops[WHITE] |
                      m_Rooks[WHITE] |
                      m_Queens[WHITE] |
                      m_Kings[WHITE];

    m_Pieces[BLACK] = m_Pawns[BLACK] |
                      m_Knights[BLACK] |
                      m_Bishops[BLACK] |
                      m_Rooks[BLACK] |
                      m_Queens[BLACK] |
                      m_Kings[BLACK]; 


    int pos_start=pos_end+1;


    if(fen[pos_start]=='w'){
        m_ColorToMove = WHITE;
    } else if(fen[pos_start]=='b'){
        m_ColorToMove = BLACK;
    }else{
        assert(false && "FEN string invalid: color to move");
    }

    pos_start+=2;

    pos_end = fen.find(' ',pos_start);

    for(int i=pos_start;i<pos_end;i++){
        char c = fen[i];
        switch (c) {
            case 'K': m_CastlingRights |= WHITE_KING_SIDE; break; // white king side castle
            case 'Q': m_CastlingRights |= WHITE_QUEEN_SIDE; break; // white queen side castle
            case 'k': m_CastlingRights |= BLACK_KING_SIDE; break; // black king side castle
            case 'q': m_CastlingRights |= BLACK_QUEEN_SIDE; break; // black queen side castle
        }
    }

    pos_start=pos_end+1;
    pos_end = fen.find(' ',pos_start);

    if(fen[pos_start]!='-'){
        std::string ep_str = fen.substr(pos_start, pos_end - pos_start);
        Square ep_sq = str_to_square(ep_str);
        if(square_to_rank(ep_sq) == RANK_3 && m_ColorToMove == BLACK){
            m_Enpassant = static_cast<Enpassant>((ep_sq << 1) + 1);
        } else if (square_to_rank(ep_sq) == RANK_6 && m_ColorToMove == WHITE){
            m_Enpassant = static_cast<Enpassant>((ep_sq << 1) + 1);
        }
    }

    pos_start=pos_end+1;
    pos_end = fen.find(' ',pos_start);

    m_HalfmoveClock = std::stoi(fen.substr(pos_start, pos_end - pos_start));

    pos_start=pos_end+1;
    pos_end = fen.find(' ',pos_start);

    m_FullmoveNumber = std::stoi(fen.substr(pos_start, pos_end - pos_start));   

}


void Board::clear() {

   
    //TODO: maybe use memeset?
    memset(this, 0, sizeof(Board));

    m_Occupancy = 0;
    for (int i = 0; i < COLOR_NUM; ++i) {
        m_Pieces[i] = 0;
        m_Pawns[i] = 0;
        m_Knights[i] = 0;
        m_Bishops[i] = 0;
        m_Rooks[i] = 0;
        m_Queens[i] = 0;
        m_Kings[i] = 0;
    }
    m_ColorToMove = WHITE;
    m_CastlingRights = CASTLE_NONE;
    m_Enpassant = EP_NONE;
    m_HalfmoveClock = 0;
    m_FullmoveNumber = 0;
}

std::string Board::get_board_info() const{
    std::string info;
    info += "Board Info:\n";
    info += "Occupancy:\n" + print_bitboard(m_Occupancy) + "\n";
    info += "White Pieces:\n" + print_bitboard(m_Pieces[WHITE]) + "\n";
    info += "Black Pieces:\n" + print_bitboard(m_Pieces[BLACK]) + "\n";
    info += "White Pawns:\n" + print_bitboard(m_Pawns[WHITE]) + "\n";
    info += "Black Pawns:\n" + print_bitboard(m_Pawns[BLACK]) + "\n";
    info += "White Knights:\n" + print_bitboard(m_Knights[WHITE]) + "\n";
    info += "Black Knights:\n" + print_bitboard(m_Knights[BLACK]) + "\n";
    info += "White Bishops:\n" + print_bitboard(m_Bishops[WHITE]) + "\n";
    info += "Black Bishops:\n" + print_bitboard(m_Bishops[BLACK]) + "\n";
    info += "White Rooks:\n" + print_bitboard(m_Rooks[WHITE]) + "\n";
    info += "Black Rooks:\n" + print_bitboard(m_Rooks[BLACK]) + "\n";
    info += "White Queens:\n" + print_bitboard(m_Queens[WHITE]) + "\n";
    info += "Black Queens:\n" + print_bitboard(m_Queens[BLACK]) + "\n";
    info += "White Kings:\n" + print_bitboard(m_Kings[WHITE]) + "\n";
    info += "Black Kings:\n" + print_bitboard(m_Kings[BLACK]) + "\n";

    info += "Color to move: " + std::string((m_ColorToMove == WHITE) ? "White" : "Black") + "\n";

    info += "Castling Rights: ";
    if (m_CastlingRights == CASTLE_NONE) {
        info += "None\n";
    } else {
        if (m_CastlingRights & WHITE_KING_SIDE) info += "K ";
        if (m_CastlingRights & WHITE_QUEEN_SIDE) info += "Q ";
        if (m_CastlingRights & BLACK_KING_SIDE) info += "k ";
        if (m_CastlingRights & BLACK_QUEEN_SIDE) info += "q ";
        info += "\n";
    }
    info += "Enpassant Square: ";
    if (m_Enpassant == EP_NONE) {
        info += "None\n";
    } else {
        Square ep_sq = static_cast<Square>((m_Enpassant - 1) >> 1);
        info += square_to_str(ep_sq) + "\n";
    }
    info += "Halfmove Clock: " + std::to_string(m_HalfmoveClock) + "\n";
    info += "Fullmove Number: " + std::to_string(m_FullmoveNumber) + "\n";

    return info;
}




Square str_to_square(const std::string& str){
    assert(str.length() == 2);
    char file_char = str[0];
    char rank_char = str[1];

    assert(file_char >= 'a' && file_char <= 'h');
    assert(rank_char >= '1' && rank_char <= '8');

    File f = static_cast<File>(file_char - 'a');
    Rank r = static_cast<Rank>(rank_char - '1');

    return rank_file_to_square(r, f);
}

std::string Board::get_board_pretty_bb() const {
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
            Square sq = rank_file_to_square(r, f);
            BitBoard mask = get_mask(sq);
            if(m_Pawns[WHITE] & mask) {
                board_str += "| P ";
            } else if(m_Pawns[BLACK] & mask) {
                board_str += "| p ";
            } else if(m_Knights[WHITE] & mask) {
                board_str += "| N ";
            } else if(m_Knights[BLACK] & mask) {
                board_str += "| n ";
            } else if(m_Bishops[WHITE] & mask) {
                board_str += "| B ";
            } else if(m_Bishops[BLACK] & mask) {
                board_str += "| b ";
            } else if(m_Rooks[WHITE] & mask) {
                board_str += "| R ";
            } else if(m_Rooks[BLACK] & mask) {
                board_str += "| r ";
            } else if(m_Queens[WHITE] & mask) {
                board_str += "| Q ";
            } else if(m_Queens[BLACK] & mask) {
                board_str += "| q ";
            } else if(m_Kings[WHITE] & mask) {
                board_str += "| K ";
            } else if(m_Kings[BLACK] & mask) {
                board_str += "| k ";
            } else {
                board_str += "|   ";
            }


        }
        board_str += "|\n";
        board_str += "---+---+---+---+---+---+---+---+---+\n";
    }

    return board_str;
}
//this funcion uses the vector m_Board to print the board
std::string Board::get_board_pretty() const {
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
            Square sq = rank_file_to_square(r, f);
            std::string piece;
            switch(m_Board[sq]) {
                case W_PAWN: piece = "P"; break;
                case B_PAWN: piece = "p"; break;
                case W_KNIGHT: piece = "N"; break;
                case B_KNIGHT: piece = "n"; break;
                case W_BISHOP: piece = "B"; break;
                case B_BISHOP: piece = "b"; break;
                case W_ROOK: piece = "R"; break;
                case B_ROOK: piece = "r"; break;
                case W_QUEEN: piece = "Q"; break;
                case B_QUEEN: piece = "q"; break;
                case W_KING: piece = "K"; break;
                case B_KING: piece = "k"; break;
                default: piece = " "; break;
            }

            board_str += "| " + piece + " ";
        }
        board_str += "|\n";
        board_str += "---+---+---+---+---+---+---+---+---+\n";
    }

    return board_str;
}
std::string square_to_str(Square sq) {
    assert(sq >= S_FIRST && sq <= S_LAST);
    File f = square_to_file(sq);
    Rank r = square_to_rank(sq);
    return std::string(1, 'a' + static_cast<char>(f)) + std::string(1, '1' + static_cast<char>(r));
}




//helper function to print a bitboard
std::string print_bitboard(BitBoard bb) {
    std::string result;
    for (Rank r = RANK_1; r <= RANK_8; ++r) {
        for (File f = FILE_A; f <= FILE_H; ++f) {
            Square sq = rank_file_to_square(r, f);
            result += (bb & (1ULL << sq)) ? square_to_str(sq) : " 0";
            result += ' ';
        }
        result += '\n';
    }
    return result;
}






} // namespace BiggerBotChess