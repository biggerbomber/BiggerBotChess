#include "board.h"
#include "movegenerator.h"
#include <cassert>
#include <chrono>
#include <cstring>

namespace BiggerBotChess {

namespace ZHash
{

Key pph[PIECE_NUM][S_NUM];
Key blackTurn;
Key castlingRights[16];
Key enpassantFile[FILE_NUM]; 

// xorshift64star Pseudo-Random Number Generator
// This class is based on original code written and dedicated
// to the public domain by Sebastiano Vigna (2014).
// It has the following characteristics:
//
//  -  Outputs 64-bit numbers
//  -  Passes Dieharder and SmallCrush test batteries
//  -  Does not require warm-up, no zeroland to escape
//  -  Internal state is a single 64-bit integer
//  -  Period is 2^64 - 1
//  -  Speed: 1.60 ns/call (Core i7 @3.40GHz)
//
// For further analysis see
//   <http://vigna.di.unimi.it/ftp/papers/xorshift.pdf>

class PRNG {

    uint64_t s;

    uint64_t rand64() {

        s ^= s >> 12, s ^= s << 25, s ^= s >> 27;
        return s * 2685821657736338717LL;
    }

   public:
    PRNG(uint64_t seed) :
        s(seed) {
        assert(seed);
    }

    template<typename T>
    T rand() {
        return T(rand64());
    }

};
void init(){

    //initialize zobrist hash keys
    PRNG prng(0xF00DDEADBEEFCAFE);
    for(int p = PAWN; p < PIECE_NUM; ++p){
        for(Square s = S_FIRST; s < S_LAST; ++s){
            pph[p][s] = prng.rand<Key>();
        }
    }

    blackTurn = prng.rand<Key>();

    for(int i =0; i <16; ++i){
        ZHash::castlingRights[i] = prng.rand<Key>();
    }

    for(int f = FILE_A; f <= FILE_H; ++f){
        enpassantFile[f] = prng.rand<Key>();
    }

}
} // namespace ZHash

Castling Board::m_CastelingMask[S_NUM] = {CASTLE_NONE};

void Board::init() {

    //init castling masks
     for(Square s = S_FIRST; s <= S_LAST; ++s){
        
        m_CastelingMask[s] = CASTLE_NONE;;
        
    }
    m_CastelingMask[S_E1] = WHITE_KING_SIDE | WHITE_QUEEN_SIDE;
    m_CastelingMask[S_H1] = WHITE_KING_SIDE;
    m_CastelingMask[S_A1] = WHITE_QUEEN_SIDE;
    m_CastelingMask[S_E8] = BLACK_KING_SIDE | BLACK_QUEEN_SIDE;
    m_CastelingMask[S_H8] = BLACK_KING_SIDE;
    m_CastelingMask[S_A8] = BLACK_QUEEN_SIDE;
   
    ZHash::init();
}

Board::Board(   const std::string& fen,
                const std::string& moves) {
    assert(moves == ""); // moves not implemented yet
    clear();
    BoardState& st = m_StateHistory[m_StateHistoryIndex++];
    st.fullplyNumber= moves.size();
    

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
            case 'K': st.castlingRights |= WHITE_KING_SIDE; break; // white king side castle
            case 'Q': st.castlingRights |= WHITE_QUEEN_SIDE; break; // white queen side castle
            case 'k': st.castlingRights |= BLACK_KING_SIDE; break; // black king side castle
            case 'q': st.castlingRights |= BLACK_QUEEN_SIDE; break; // black queen side castle
        }
    }

    pos_start=pos_end+1;
    pos_end = fen.find(' ',pos_start);

    if(fen[pos_start]!='-'){
        std::string ep_str = fen.substr(pos_start, pos_end - pos_start);
        Square ep_sq = str_to_square(ep_str);
        if(get_rank(ep_sq) == RANK_3 && m_ColorToMove == BLACK){
            st.enpassant = static_cast<Enpassant>(ep_sq);
        } else if (get_rank(ep_sq) == RANK_6 && m_ColorToMove == WHITE){
            st.enpassant = static_cast<Enpassant>(ep_sq);
        }
    }else{
        st.enpassant = EP_NONE;
    }

    pos_start=pos_end+1;
    pos_end = fen.find(' ',pos_start);

    st.halfmoveClock = std::stoi(fen.substr(pos_start, pos_end - pos_start));

    pos_start=pos_end+1;
    pos_end = fen.find(' ',pos_start);

    st.fullplyNumber = std::stoi(fen.substr(pos_start, pos_end - pos_start))*2 + (m_ColorToMove == BLACK ? 1 : 0);   
    init_zobrist_key();
}

void Board::init_zobrist_key() {
    m_ZobristKey = 0;
    for (Square sq = S_FIRST; sq <= S_LAST; ++sq) {
        Piece p = m_Board[sq];
        if (p != NONE_PIECE) {
            m_ZobristKey ^= ZHash::pph[get_piece_type(p)][sq];
        }
    }

    if (m_ColorToMove == BLACK) {
        m_ZobristKey ^= ZHash::blackTurn;
    }

    BoardState& st = m_StateHistory[m_StateHistoryIndex - 1];
    m_ZobristKey ^= ZHash::castlingRights[st.castlingRights];

    if (st.enpassant != EP_NONE) {
        File ep_file = get_file(static_cast<Square>(st.enpassant));
        m_ZobristKey ^= ZHash::enpassantFile[ep_file];
    }
}

void Board::clear() {

    m_StateHistoryIndex = 0;
    memset(m_Board, NONE_PIECE, sizeof(m_Board));
   
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
}

std::string Board::get_board_info() const{
    BoardState st = m_StateHistory[m_StateHistoryIndex-1];
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
    if (st.castlingRights == CASTLE_NONE) {
        info += "None\n";
    } else {
        if (st.castlingRights & WHITE_KING_SIDE) info += "K ";
        if (st.castlingRights & WHITE_QUEEN_SIDE) info += "Q ";
        if (st.castlingRights & BLACK_KING_SIDE) info += "k ";
        if (st.castlingRights & BLACK_QUEEN_SIDE) info += "q ";
        info += "\n";
    }
    info += "Enpassant Square: ";
    if (st.enpassant == EP_NONE) {
        info += "None\n";
    } else {
        info += square_to_str(static_cast<Square>(st.enpassant)) + "\n";
    }
    info += "Halfmove Clock: " + std::to_string(st.halfmoveClock) + "\n";
    info += "Fullmove Number: " + std::to_string(st.fullplyNumber) + "\n";

    info += "Zobrist Key: " + std::to_string(m_ZobristKey) + "\n";

    return info;
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
    attackers |= BB::get_attacks<KNIGHT>(s, occ) & m_Knights[by];
    if (attackers) return true;

    // Bishops and Queens (diagonal attacks)
    attackers |= BB::get_attacks<BISHOP>(s, occ) & (m_Bishops[by] | m_Queens[by]);
    if (attackers) return true;

    // Rooks and Queens (straight attacks)
    attackers |= BB::get_attacks<ROOK>(s, occ) & (m_Rooks[by] | m_Queens[by]);
    if (attackers) return true;

    // Kings
    attackers |= BB::get_attacks<KING>(s , occ) & m_Kings[by];
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
    const BoardState& st = m_StateHistory[m_StateHistoryIndex-1];
    assert(side == KING_SIDE || side == QUEEN_SIDE);

    side = static_cast<Castling>(side << (c * 2)); // shift side to the correct color

    switch (side)
    {
        case WHITE_KING_SIDE:
            if(!(st.castlingRights & WHITE_KING_SIDE)) return false; // no castling rights
            if(get_piece_on(S_E1) != W_KING || get_piece_on(S_H1) != W_ROOK) return false; // king or rook not in place
            if(m_Occupancy & (get_mask(S_F1) | get_mask(S_G1))) return false; // squares not empty
            if(is_square_attacked(S_E1, BLACK) || is_square_attacked(S_F1, BLACK) || is_square_attacked(S_G1, BLACK)) return false; // squares attacked
            
            return true;
        case WHITE_QUEEN_SIDE:
            if(!(st.castlingRights & WHITE_QUEEN_SIDE)) return false; // no castling rights
            if(get_piece_on(S_E1) != W_KING || get_piece_on(S_A1) != W_ROOK) return false; // king or rook not in place
            if(m_Occupancy & (get_mask(S_B1) | get_mask(S_C1) | get_mask(S_D1))) return false; // squares not empty
            if(is_square_attacked(S_E1, BLACK) || is_square_attacked(S_D1, BLACK) || is_square_attacked(S_C1, BLACK)) return false; // squares attacked
            return true;
        case BLACK_KING_SIDE:
            if(!(st.castlingRights & BLACK_KING_SIDE)) return false; // no castling rights
            if(get_piece_on(S_E8) != B_KING || get_piece_on(S_H8) != B_ROOK) return false; // king or rook not in place
            if(m_Occupancy & (get_mask(S_F8) | get_mask(S_G8))) return false; // squares not empty
            if(is_square_attacked(S_E8, WHITE) || is_square_attacked(S_F8, WHITE) || is_square_attacked(S_G8, WHITE)) return false; // squares attacked
            return true;
        case BLACK_QUEEN_SIDE:
            if(!(st.castlingRights & BLACK_QUEEN_SIDE)) return false; // no castling rights
            if(get_piece_on(S_E8) != B_KING || get_piece_on(S_A8) != B_ROOK) return false; // king or rook not in place
            if(m_Occupancy & (get_mask(S_B8) | get_mask(S_C8) | get_mask(S_D8))) return false; // squares not empty
            if(is_square_attacked(S_E8, WHITE) || is_square_attacked(S_D8, WHITE) || is_square_attacked(S_C8, WHITE)) return false; // squares attacked
            return true;
    
    default:
        assert(false && "Invalid castling side");
        break;
    }
    return false;
}

void Board::do_castle(Castling side, bool undo) {
    BoardState& st_old = m_StateHistory[m_StateHistoryIndex-2];
    BoardState& st = m_StateHistory[m_StateHistoryIndex-1];
    Color c = get_color();
    
    Castling full_side = static_cast<Castling>(side << (c * 2)); 

    Square king_start=S_A1, king_end=S_A1, rook_start=S_A1, rook_end=S_A1;
    if(c == WHITE) {
        if(full_side == WHITE_KING_SIDE) {
            king_start = S_E1; king_end = S_G1;
            rook_start = S_H1; rook_end = S_F1;
        } else if(full_side == WHITE_QUEEN_SIDE) {
            king_start = S_E1; king_end = S_C1;
            rook_start = S_A1; rook_end = S_D1;
        } else {
            assert(false && "Invalid castling side for white");
        }
    } else {
        if(full_side == BLACK_KING_SIDE) {
            king_start = S_E8; king_end = S_G8;
            rook_start = S_H8; rook_end = S_F8;
        } else if(full_side == BLACK_QUEEN_SIDE) {
            king_start = S_E8; king_end = S_C8;
            rook_start = S_A8; rook_end = S_D8;
        } else {
            assert(false && "Invalid castling side for black");
        }
    }

    if(undo){
        std::swap(king_start, king_end);
        std::swap(rook_start, rook_end);
    }

    // Move the king
    Piece king = get_piece_on(king_start);
    m_Board[king_start] = NONE_PIECE;
    m_Board[king_end] = king;

    m_Kings[c] &= ~get_mask(king_start);
    m_Kings[c] |= get_mask(king_end);
    m_Pieces[c] &= ~get_mask(king_start);
    m_Pieces[c] |= get_mask(king_end);


    // Move the rook
    Piece rook = get_piece_on(rook_start);
    m_Board[rook_start] = NONE_PIECE;
    m_Board[rook_end] = rook;

    m_Rooks[c] &= ~get_mask(rook_start);
    m_Rooks[c] |= get_mask(rook_end);
    m_Pieces[c] &= ~get_mask(rook_start);
    m_Pieces[c] |= get_mask(rook_end);

    update_occupancy();
    // Update castling rights
    if(!undo) {
        //m_CastlingRightsList[m_CastlingRightsListIndex++] = m_CastlingRights;
        st.castlingRights = static_cast<Castling>(st_old.castlingRights & (c == WHITE ? ~WHITE_SIDE : ~BLACK_SIDE));
        // Update en passant square
        st.enpassant = EP_NONE;

        st.capturedPiece = NONE_PIECE; // No piece captured during castling
    }
}


void Board::unsafe_do_move(const Move& m) {


    assert(m_StateHistoryIndex > 0 && "No board state to copy from");
    Square start = m.get_start();
    Square dest = m.get_dest();
    MoveType type = m.get_type();
    Color c = get_color();
    BoardState& old_st = m_StateHistory[m_StateHistoryIndex-1];
    BoardState& st = m_StateHistory[m_StateHistoryIndex++];

    // copy old state
    st.move = m;
    st.capturedPiece = NONE_PIECE;
    st.castlingRights = old_st.castlingRights;
    st.enpassant = EP_NONE;
    st.halfmoveClock = old_st.halfmoveClock+1;
    st.fullplyNumber = old_st.fullplyNumber + 1;

    if(type ==NORMAL)
        {
            Piece dest_cap = get_piece_on(dest);
            st.capturedPiece=dest_cap;
            if(dest_cap != NONE_PIECE){
                st.halfmoveClock = 0;

                PieceType captured_type = get_piece_type(dest_cap);

                BitBoard& occ_cap_pieces = get_pieces_ref(~c, captured_type);
                BitBoard& occ_all_pieces = get_pieces_ref(~c, ALL_PIECES);
                occ_cap_pieces &= ~get_mask(dest);
                occ_all_pieces &= ~get_mask(dest);
                
                m_ZobristKey ^= ZHash::pph[captured_type][dest];
            }


            Piece moving_piece = get_piece_on(start);
            m_Board[start] = NONE_PIECE;
            m_Board[dest] = moving_piece;

            PieceType moving_type = get_piece_type(moving_piece);

            BitBoard& occ_moving_pieces = get_pieces_ref(c, moving_type);
            BitBoard& occ_all_pieces = get_pieces_ref(c, ALL_PIECES);

            occ_moving_pieces &= ~get_mask(start);
            occ_moving_pieces |= get_mask(dest);
            occ_all_pieces &= ~get_mask(start);
            occ_all_pieces |= get_mask(dest);
            update_occupancy();

            if(moving_type == PAWN){
                st.halfmoveClock = 0; // reset halfmove clock on pawn move or capture
                if(abs(dest - start) == 16) { // double pawn move
                    st.enpassant = static_cast<Enpassant>((start + dest) / 2);
                }
            }

            // Update castling rights if needed
            //if(st.castlingRights & (m_CastelingMask[start] | m_CastelingMask[dest])) {
                st.castlingRights = static_cast<Castling>(st.castlingRights & ~(m_CastelingMask[start] | m_CastelingMask[dest]));
            //}

            m_ZobristKey ^= ZHash::pph[moving_type][start];
            m_ZobristKey ^= ZHash::pph[moving_type][dest];

        }else if(type == CASTLE)
        {
            Castling side = (dest > start) ? KING_SIDE : QUEEN_SIDE;
            //std::cout << "doinug castle\n";
            do_castle(side);
        
        }
        else if(type== EN_PASSANT)
        {
            assert(old_st.enpassant != EP_NONE && "En passant move but no en passant square set");
            assert(dest == static_cast<Square>(old_st.enpassant) && "En passant move to wrong square");
            // Move the pawn
            Piece pawn = get_piece_on(start);
            m_Board[start] = NONE_PIECE;
            m_Board[dest] = pawn;
            m_Pawns[c] &= ~get_mask(start);
            m_Pieces[c] &= ~get_mask(start);

            m_Pawns[c] |= get_mask(dest);
            m_Pieces[c] |= get_mask(dest);
            // Remove the captured pawn
            Direction ep_capture_dir = (c == WHITE) ? SOUTH : NORTH;
            Square captured_sq = static_cast<Square>(dest + ep_capture_dir);
            Piece captured_pawn = get_piece_on(captured_sq);
            m_Board[captured_sq] = NONE_PIECE;
            m_Pawns[~c] &= ~get_mask(captured_sq);
            m_Pieces[~c] &= ~get_mask(captured_sq);
            st.capturedPiece = captured_pawn;
            st.halfmoveClock = 0; // reset halfmove clock on capture
            // Clear en passant square
            st.enpassant = EP_NONE;
            update_occupancy();
        }
        else if(type ==PROMOTION)
        {
            Piece dest_cap = get_piece_on(dest);
            st.capturedPiece=dest_cap;
            st.halfmoveClock = 0;
            if(dest_cap != NONE_PIECE){
                st.halfmoveClock = 0;
                PieceType captured_type = get_piece_type(dest_cap);
                BitBoard& occ_cap_pieces = get_pieces_ref(~c, captured_type);

                BitBoard& occ_all_pieces = get_pieces_ref(~c, ALL_PIECES);


                occ_cap_pieces &= ~get_mask(dest);
                occ_all_pieces &= ~get_mask(dest);
            }
            

            PieceType promo_type = m.get_promotion_piece();
            Piece promo_piece = make_piece(c, promo_type);

            
            m_Board[start] = NONE_PIECE;
            m_Board[dest] = promo_piece;

            m_Pawns[c] &= ~get_mask(start);
            m_Pieces[c] &= ~get_mask(start);
         
            BitBoard& occ_prom = get_pieces_ref(c, promo_type);
            occ_prom |= get_mask(dest);
            m_Pieces[c] |= get_mask(dest);

            update_occupancy();
            // Clear en passant square
        }
        m_ColorToMove = ~m_ColorToMove;
}
    
   


bool Board::is_legal(const Move& m){
    unsafe_do_move(m);
    Color c = ~get_color(); // color that just moved
    bool in_check = is_square_attacked(lsb(m_Kings[c]), get_color());
    undo_move();
    return !in_check;
}

bool Board::do_move(const Move& m, bool trust_legal) {
    unsafe_do_move(m);
    if(trust_legal){
        return true;
    }else{
        Color c = ~get_color(); // color that just moved
        bool in_check = is_square_attacked(lsb(m_Kings[c]), get_color());
        if(!in_check){
            return true;
        }
        undo_move();
    }

    return false;
}

void Board::undo_move() {
    assert((m_StateHistoryIndex > 1 ) && "No moves to undo");

    BoardState& st = m_StateHistory[m_StateHistoryIndex-1];
    m_StateHistoryIndex--;
    const Move& m =st.move;
    
    Square start = m.get_start();
    Square dest = m.get_dest();
    MoveType type = m.get_type();

    m_ColorToMove = ~m_ColorToMove;

    switch(type){
        case NORMAL:
        {
            Piece captured_piece = st.capturedPiece;
            

            if(captured_piece != NONE_PIECE){
                PieceType captured_type = get_piece_type(captured_piece);
                Color c = ~m_ColorToMove; // color of the captured piece
                BitBoard& occ_cap_pieces = get_pieces_ref(c, captured_type);
                occ_cap_pieces |= get_mask(dest);
                BitBoard& occ_all_pieces = get_pieces_ref(c, ALL_PIECES);
                occ_all_pieces |= get_mask(dest);

                m_ZobristKey ^= ZHash::pph[captured_type][dest];
            }

            Piece moving_piece = get_piece_on(dest);
            m_Board[dest] = captured_piece;
            m_Board[start] = moving_piece;

            PieceType moving_type = get_piece_type(moving_piece);
            Color c = m_ColorToMove; // color of the piece that moved
            BitBoard& occ_moving_pieces = get_pieces_ref(c, moving_type);
            BitBoard& occ_all_pieces = get_pieces_ref(c, ALL_PIECES);
            occ_moving_pieces &= ~get_mask(dest);
            occ_moving_pieces |= get_mask(start);
            occ_all_pieces &= ~get_mask(dest);
            occ_all_pieces |= get_mask(start);

            update_occupancy();

            m_ZobristKey ^= ZHash::pph[moving_type][start];
            m_ZobristKey ^= ZHash::pph[moving_type][dest];

        }
            break;
        case CASTLE:
        {
            Castling side = (dest > start) ? KING_SIDE : QUEEN_SIDE;
            do_castle(side, true);
            break;
        }
        case EN_PASSANT:
        {
            // Move the pawn back
            Color c = m_ColorToMove; // color of the pawn that moved
            Piece pawn = get_piece_on(dest);
            m_Board[dest] = NONE_PIECE;
            m_Board[start] = pawn;
            m_Pawns[c] &= ~get_mask(dest);
            m_Pawns[c] |= get_mask(start);

            m_Pieces[c] &= ~get_mask(dest);
            m_Pieces[c] |= get_mask(start);
            // Restore the captured pawn
            Piece captured_pawn = st.capturedPiece;
        
            Direction ep_capture_dir = (c == WHITE) ? SOUTH : NORTH;
            Square captured_sq = static_cast<Square>(dest + ep_capture_dir);
            m_Board[captured_sq] = captured_pawn;
            m_Pawns[~c] |= get_mask(captured_sq);
            // Restore en passant square
            m_Pieces[~c] |= get_mask(captured_sq);
            
            update_occupancy();

        }
            break;
        case PROMOTION:
        {
            //std::cout << "Undoing promotion from " << square_to_str(start) << " to " << square_to_str(dest) << std::endl;
            Color c = m_ColorToMove; // color of the pawn that moved
            Piece captured_piece = st.capturedPiece;
            

            Piece promo_piece = get_piece_on(dest);
            m_Board[dest] = captured_piece;
            m_Board[start] = make_piece(c, PAWN);
            m_Pawns[c] |= get_mask(start);
            m_Pieces[c] |= get_mask(start);
           
            if(captured_piece != NONE_PIECE) {
                PieceType captured_type = get_piece_type(captured_piece);
                BitBoard& occ_cap_pieces = get_pieces_ref(~c, captured_type);
                occ_cap_pieces |= get_mask(dest);
                BitBoard& occ_all_pieces = get_pieces_ref(~c, ALL_PIECES);
                occ_all_pieces |= get_mask(dest);
            }

            PieceType promo_type = get_piece_type(promo_piece);
            BitBoard& occ_prom = get_pieces_ref(c, promo_type);
            occ_prom &= ~get_mask(dest);
            m_Pieces[c] &= ~get_mask(dest);
        
            // Restore en passant square
            update_occupancy();


        }
            break;
        default:
        {
            assert(false && "Invalid move type");
        }
    }
    update_occupancy();
    
}

void Board::sanity_check(){
    BitBoard Pieces_Black = m_Pawns[BLACK]
                        | m_Knights[BLACK]
                        | m_Bishops[BLACK]
                        | m_Rooks[BLACK]
                        | m_Queens[BLACK]
                        | m_Kings[BLACK];
    BitBoard Pieces_White = m_Pawns[WHITE]
                        | m_Knights[WHITE]
                        | m_Bishops[WHITE]  
                        | m_Rooks[WHITE]
                        | m_Queens[WHITE]
                        | m_Kings[WHITE];
    assert(Pieces_Black == m_Pieces[BLACK] && "Black pieces bitboard mismatch");
    assert(Pieces_White == m_Pieces[WHITE] && "White pieces bitboard mismatch");
    assert((Pieces_Black & Pieces_White) == 0 && "Black and White pieces overlap");

    for(PieceType pt = PAWN; pt <= KING; pt = static_cast<PieceType>(pt + 1)) {
        for(PieceType pt2 = PAWN; pt2 <= KING; pt2 = static_cast<PieceType>(pt2 + 1)) {
            if(pt != pt2) {
                BitBoard bb1 = get_pieces(WHITE, pt);
                BitBoard bb2 = get_pieces(WHITE, pt2);
                assert((bb1 & bb2) == 0 && "White piece type bitboards overlap");

                bb1 = get_pieces_ref(BLACK, pt);
                bb2 = get_pieces_ref(BLACK, pt2);
                assert((bb1 & bb2) == 0 && "Black piece type bitboards overlap");
            }
        }
    }

    BitBoard Occupancy = Pieces_Black | Pieces_White;
    assert(Occupancy == m_Occupancy && "Occupancy bitboard mismatch");

    //there must be exactly one king of each color
    assert(pop_count(m_Kings[WHITE]) == 1 && "White must have exactly one king");
    assert(pop_count(m_Kings[BLACK]) == 1 && "Black must have exactly one king");

    //check that the pieces on the board match the bitboards

    for(Square sq = S_A1; sq <= S_H8; ++sq) {
        Piece p = m_Board[sq];
        if(p != NONE_PIECE) {
            Color c = get_color_of(p);
            PieceType pt = get_piece_type(p);
            BitBoard bb = get_pieces(c, pt);
            assert((bb & get_mask(sq)) != 0 && "Piece on board but bitboard mismatch");
        }else{
            //empty square
            assert(((m_Occupancy & get_mask(sq)) == 0) && "Empty square but occupancy bitboard mismatch");
        }
    }

}

void Board::update_occupancy() {
    m_Occupancy = m_Pieces[WHITE] | m_Pieces[BLACK];
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

Move Board::str_to_move(const std::string& move_str) const {
    //regex to match move strings 
    std::regex move_regex("([a-h][1-8])([a-h][1-8])([nbrq])?");
    std::smatch sm;
    if(!std::regex_search(move_str,sm, move_regex)) {
        return Move::null();
    }
    Square start = str_to_square(sm[1]);
    Square dest = str_to_square(sm[2]);

    PieceType promo_piece = NONE;
    if(sm[3].matched) {
        char promo_char = sm[3].str()[0];
        switch(promo_char) {
            case 'n': promo_piece = KNIGHT; break;
            case 'b': promo_piece = BISHOP; break;
            case 'r': promo_piece = ROOK; break;
            case 'q': promo_piece = QUEEN; break;
            default: return Move::null();
        }
    }

    MoveSaver move_saver(*this,LEGAL);

    for(const Move& m : move_saver){
        if(m.get_start() == start && m.get_dest() == dest) {
            if(m.get_type() == PROMOTION) {
                if(m.get_promotion_piece() == promo_piece) {
                    return m;
                }
            } else {
                if(promo_piece == NONE) {
                    return m;
                }
            }
        }
    }

    return Move::null();
}

} // namespace BiggerBotChess