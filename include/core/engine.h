#pragma once

#include <memory>
#include <string>

namespace chess {

class Board;
class MoveGenerator;
class Search;
class Evaluator;

/**
 * @brief Main chess engine class that coordinates all components
 */
class Engine {
public:
    Engine();
    ~Engine();

    /**
     * @brief Initialize the engine with default settings
     */
    void initialize();

    /**
     * @brief Set position from FEN string
     * @param fen FEN notation string
     */
    void setPosition(const std::string& fen);

    /**
     * @brief Find the best move for the current position
     * @param maxDepth Maximum search depth
     * @return Best move in algebraic notation
     */
    std::string findBestMove(int maxDepth = 6);

    /**
     * @brief Make a move on the board
     * @param move Move in algebraic notation
     * @return True if move was valid and made
     */
    bool makeMove(const std::string& move);

    /**
     * @brief Get current board state as FEN
     * @return FEN string
     */
    std::string getCurrentPosition() const;

    /**
     * @brief Check if the game is over
     * @return True if game is over (checkmate, stalemate, etc.)
     */
    bool isGameOver() const;

private:
    std::unique_ptr<Board> m_board;
    std::unique_ptr<MoveGenerator> m_moveGenerator;
    std::unique_ptr<Search> m_search;
    std::unique_ptr<Evaluator> m_evaluator;
};

} // namespace chess