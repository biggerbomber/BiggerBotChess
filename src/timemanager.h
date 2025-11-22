#include "types.h"
#include <chrono>
namespace BiggerBotChess {


class Timemanager
{

public:

    inline void start_timer(){
        m_StartTime = std::chrono::high_resolution_clock::now();
    }

    inline bool time_to_stop() const{
        auto now = std::chrono::high_resolution_clock::now();
        int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_StartTime).count();
        return (elapsed >= m_MoveTime);
    }

    Timemanager(int wtime, int btime, int winc, int binc, Color to_move)
    {
        if(to_move == WHITE){
            m_TimeLeft = wtime;
            m_Inc = winc;
        } else {
            m_TimeLeft = btime;
            m_Inc = binc;
        }

        m_MoveTime = m_TimeLeft /20 + m_Inc/2; 
    }
private:
    
    int m_TimeLeft; 
    int m_Inc;
    int m_MoveTime;

    std::chrono::high_resolution_clock::time_point m_StartTime;
};



} // namespace BiggerBotChess