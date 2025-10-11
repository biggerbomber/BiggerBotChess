#include "board.h"
#include <cassert>
#include <cstring>

namespace BiggerBotChess {


void Board::init() {
   //Init
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
        Square sq = get_square(r, f);
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
        if(get_rank(ep_sq) == RANK_3 && m_ColorToMove == BLACK){
            m_Enpassant = static_cast<Enpassant>((ep_sq << 1) + 1);
        } else if (get_rank(ep_sq) == RANK_6 && m_ColorToMove == WHITE){
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


BitBoard Board::get_pieces(Color c, PieceType p){
    switch (p)
    {
        case PAWN: return m_Pawns[c];
        case KNIGHT: return m_Knights[c];
        case BISHOP: return m_Bishops[c];
        case ROOK: return m_Rooks[c];
        case QUEEN: return m_Queens[c];
        case KING: return m_Kings[c];
        case ALL_PIECES: return m_Pieces[c];
        default: return 0;
    }
}

Piece Board::make_piece(Color c, PieceType p){
    return static_cast<Piece>(p + (c << BLACK_PIECE_OFFSET));
}

bool Board::is_square_attacked(Square s, Color by) const {
    BitBoard attackers = 0;
    BitBoard occ = m_Occupancy;

    // Pawns
    attackers |= BB::get_pawn_attacks(s, ~by) & m_Pawns[by];
    if (attackers) return true;

    // Knights
    attackers |= BB::get_attacks(s, KNIGHT, occ) & m_Knights[by];
    if (attackers) return true;

    // Bishops and Queens (diagonal attacks)
    attackers |= BB::get_attacks(s, BISHOP, occ) & (m_Bishops[by] | m_Queens[by]);
    if (attackers) return true;

    // Rooks and Queens (straight attacks)
    attackers |= BB::get_attacks(s, ROOK, occ) & (m_Rooks[by] | m_Queens[by]);
    if (attackers) return true;

    // Kings
    attackers |= BB::get_attacks(s, KING, occ) & m_Kings[by];
    if (attackers) return true;

    return false;

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
            Square sq = get_square(r, f);
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


bool Board::is_castle_possible(Color c, Castling side) const{

    assert(side == KING_SIDE || side == QUEEN_SIDE);

    side = static_cast<Castling>(side << (c * 2)); // shift side to the correct color

    switch (side)
    {
        case WHITE_KING_SIDE:
            if(!(m_CastlingRights & WHITE_KING_SIDE)) return false; // no castling rights
            if(get_piece_on(S_E1) != W_KING || get_piece_on(S_H1) != W_ROOK) return false; // king or rook not in place
            if(m_Occupancy & (get_mask(S_F1) | get_mask(S_G1))) return false; // squares not empty
            if(is_square_attacked(S_E1, BLACK) || is_square_attacked(S_F1, BLACK) || is_square_attacked(S_G1, BLACK)) return false; // squares attacked
            
            return true;
        case WHITE_QUEEN_SIDE:
            if(!(m_CastlingRights & WHITE_QUEEN_SIDE)) return false; // no castling rights
            if(get_piece_on(S_E1) != W_KING || get_piece_on(S_A1) != W_ROOK) return false; // king or rook not in place
            if(m_Occupancy & (get_mask(S_B1) | get_mask(S_C1) | get_mask(S_D1))) return false; // squares not empty
            if(is_square_attacked(S_E1, BLACK) || is_square_attacked(S_D1, BLACK) || is_square_attacked(S_C1, BLACK)) return false; // squares attacked
            return true;
        case BLACK_KING_SIDE:
            if(!(m_CastlingRights & BLACK_KING_SIDE)) return false; // no castling rights
            if(get_piece_on(S_E8) != B_KING || get_piece_on(S_H8) != B_ROOK) return false; // king or rook not in place
            if(m_Occupancy & (get_mask(S_F8) | get_mask(S_G8))) return false; // squares not empty
            if(is_square_attacked(S_E8, WHITE) || is_square_attacked(S_F8, WHITE) || is_square_attacked(S_G8, WHITE)) return false; // squares attacked
            return true;
        case BLACK_QUEEN_SIDE:
            if(!(m_CastlingRights & BLACK_QUEEN_SIDE)) return false; // no castling rights
            if(get_piece_on(S_E8) != B_KING || get_piece_on(S_A8) != B_ROOK) return false; // king or rook not in place
            if(m_Occupancy & (get_mask(S_B8) | get_mask(S_C8) | get_mask(S_D8))) return false; // squares not empty
            if(is_square_attacked(S_E8, WHITE) || is_square_attacked(S_D8, WHITE) || is_square_attacked(S_C8, WHITE)) return false; // squares attacked
            return true;
    
    default:
        assert(false && "Invalid castling side");
        break;
    }

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
            Square sq = get_square(r, f);
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

} // namespace BiggerBotChess