#include "core/engine.h"
#include "board/board.h"
#include "move/move_generator.h"
#include "search/search.h"
#include "evaluation/evaluator.h"

namespace chess {

Engine::Engine() {
    m_board = std::make_unique<Board>();
    m_moveGenerator = std::make_unique<MoveGenerator>();
    m_evaluator = std::make_unique<Evaluator>();
    m_search = std::make_unique<Search>(m_moveGenerator.get(), m_evaluator.get());
}

Engine::~Engine() = default;

void Engine::initialize() {
    // Set up initial chess position
    m_board->setFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

void Engine::setPosition(const std::string& fen) {
    m_board->setFromFen(fen);
}

std::string Engine::findBestMove(int maxDepth) {
    Move bestMove = m_search->findBestMove(*m_board, maxDepth);
    return bestMove.toAlgebraic();
}

bool Engine::makeMove(const std::string& move) {
    Move m = Move::fromAlgebraic(move);
    return m_board->makeMove(m);
}

std::string Engine::getCurrentPosition() const {
    return m_board->toFen();
}

bool Engine::isGameOver() const {
    Color sideToMove = m_board->getSideToMove();
    return m_board->isCheckmate(sideToMove) || m_board->isStalemate(sideToMove);
}

} // namespace chess