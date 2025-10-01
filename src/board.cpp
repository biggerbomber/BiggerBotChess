#include "board.h"
#include <cassert>


namespace BiggerBotChess {

BitBoard Board::S_SQUARE_MASKS[S_NUM];
BitBoard Board::S_RANK_MASKS[RANK_NUM];
BitBoard Board::S_FILE_MASKS[FILE_NUM];

void Board::init() {
    // INIT SQUARE MASKS
    for (Square sq = S_FIRST; sq <= S_LAST; ++sq) {
        S_SQUARE_MASKS[sq] = (1ULL << sq);
    }
    // INIT RANK MASKS
    for (Rank r = RANK_FIRST; r <= RANK_LAST; ++r) {
        BitBoard rank_mask = (1ULL << 8) - 1;
        rank_mask <<= (r * 8);
        S_RANK_MASKS[r] = rank_mask;
    }
    // INIT FILE MASKS

    for (File f = FILE_FIRST; f <= FILE_LAST; ++f) {
        BitBoard file_mask = 0x101010101010101ULL;
        file_mask <<= f;
        S_FILE_MASKS[f] = file_mask;
    }
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

    int s_count = S_FIRST;
    for (int i=0;i<pos_end;i++) {
        char c  = fen[0];

        if (c == '/') {
            continue;
        }
        Square sq = rank_file_to_square(Rank(RANK_NUM - square_to_rank(Square(s_count))), square_to_file(Square(s_count)));
        ++s_count;

        switch (c) {
            case 'P': m_Pawns[WHITE] |= (1ULL << sq); break;
            case 'p': m_Pawns[BLACK] |= (1ULL << sq); break;
            case 'N': m_Knights[WHITE] |= (1ULL << sq); break;
            case 'n': m_Knights[BLACK] |= (1ULL << sq); break;
            case 'B': m_Bishops[WHITE] |= (1ULL << sq); break;
            case 'b': m_Bishops[BLACK] |= (1ULL << sq); break;
            case 'R': m_Rooks[WHITE] |= (1ULL << sq); break;
            case 'r': m_Rooks[BLACK] |= (1ULL << sq); break;
            case 'Q': m_Queens[WHITE] |= (1ULL << sq); break;
            case 'q': m_Queens[BLACK] |= (1ULL << sq); break;
            case 'K': m_Kings[WHITE] |= (1ULL << sq); break;
            case 'k': m_Kings[BLACK] |= (1ULL << sq); break;
            default:

                if (isdigit(c)) {
                    s_count += c - '0' - 1;
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
    }

    pos_start+=2;

    pos_end = fen.find(' ',pos_start);

    for(int i=pos_start;i<pos_end;i++){
        char c = fen[i];
        switch (c) {
            case 'K': break; // white king side castle
            case 'Q': break; // white queen side castle
            case 'k': break; // black king side castle
            case 'q': break; // black queen side castle
        }
    }


}


void Board::clear() {

    //TODO: maybe use memeset?
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








//helper function to print a bitboard
std::string bitboard_print(BitBoard bb) {
    std::string result;
    for (Rank r = RANK_1; r <= RANK_8; ++r) {
        for (File f = FILE_A; f <= FILE_H; ++f) {
            Square sq = rank_file_to_square(r, f);
            result += (bb & (1ULL << sq)) ? '1' : '0';
            result += ' ';
        }
        result += '\n';
    }
    return result;
}






} // namespace BiggerBotChess