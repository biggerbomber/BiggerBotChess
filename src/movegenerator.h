#pragma once
#include "board.h"
#include <cassert>

namespace BiggerBotChess
{
class MoveSaver;
constexpr int MAX_MOVES = 256;


//Capture + Stable == ALL
//The legal is only a convinience value
enum GenMoveType{
    CAPTURE,
    STABLE,
    ALL,
    LEGAL
};

void generate_moves(const Board&, const GenMoveType&, MoveSaver*);
void generate_moves_legal(Board, MoveSaver*);


class MoveSaver{
public:
    MoveSaver(const Board& b, const GenMoveType& ty){
        if(ty==LEGAL){
            generate_moves_legal(b,this);
        } else {
            generate_moves(b,ty,this);
        }
    }

    MoveSaver& operator+=(const Move& other){
        //if we exceed the max moves we are screwed, but this should never happen
        assert(m_free_pos<MAX_MOVES); // maybe reenable later
        m_data[m_free_pos++]=other;
        return *this;
    }

    const Move* begin() const{ return &m_data[0]; };
    const Move* end() const { return &m_data[0]+m_free_pos;};

    Move operator[](size_t index) const {
        assert(index < m_free_pos);
        return m_data[index];
    }

    size_t size() const { return m_free_pos;}
    bool is_empty() {return size()==0;}

    uint16_t m_free_pos = 0;
    Move m_data[MAX_MOVES];
};





} // namespace BiggerBotChess
