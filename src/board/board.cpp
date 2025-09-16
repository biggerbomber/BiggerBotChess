#include "board/board.h"
#include "move/move.h"
#include "utils/utils.h"
#include <sstream>

namespace chess {

Board::Board() {
    setupInitialPosition();
}

void Board::setupInitialPosition() {
    // Clear the board
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            m_board[row][col] = Piece();
        }
    }
    
    // Set up starting position
    setFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

void Board::setFromFen(const std::string& fen) {
    std::vector<std::string> parts = Utils::split(fen, ' ');
    if (parts.size() < 4) return;
    
    // Parse board position
    std::string position = parts[0];
    int row = 0, col = 0;
    
    for (char c : position) {
        if (c == '/') {
            row++;
            col = 0;
        } else if (std::isdigit(c)) {
            int emptySquares = c - '0';
            for (int i = 0; i < emptySquares; ++i) {
                m_board[row][col++] = Piece();
            }
        } else {
            m_board[row][col++] = Piece::fromChar(c);
        }
    }
    
    // Parse side to move
    m_sideToMove = (parts[1] == "w") ? Color::White : Color::Black;
    
    // Parse castling rights
    std::string castling = parts[2];
    m_whiteKingside = castling.find('K') != std::string::npos;
    m_whiteQueenside = castling.find('Q') != std::string::npos;
    m_blackKingside = castling.find('k') != std::string::npos;
    m_blackQueenside = castling.find('q') != std::string::npos;
    
    // Parse en passant
    if (parts[3] != "-") {
        m_enPassantFile = parts[3][0] - 'a';
    } else {
        m_enPassantFile = -1;
    }
    
    // Parse halfmove and fullmove counters
    if (parts.size() >= 5) m_halfmoveClock = std::stoi(parts[4]);
    if (parts.size() >= 6) m_fullmoveNumber = std::stoi(parts[5]);
}

std::string Board::toFen() const {
    std::stringstream ss;
    
    // Board position
    for (int row = 0; row < BOARD_SIZE; ++row) {
        int emptyCount = 0;
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (m_board[row][col].isEmpty()) {
                emptyCount++;
            } else {
                if (emptyCount > 0) {
                    ss << emptyCount;
                    emptyCount = 0;
                }
                ss << m_board[row][col].toChar();
            }
        }
        if (emptyCount > 0) {
            ss << emptyCount;
        }
        if (row < BOARD_SIZE - 1) {
            ss << '/';
        }
    }
    
    // Side to move
    ss << ' ' << (m_sideToMove == Color::White ? 'w' : 'b');
    
    // Castling rights
    ss << ' ';
    std::string castling = "";
    if (m_whiteKingside) castling += 'K';
    if (m_whiteQueenside) castling += 'Q';
    if (m_blackKingside) castling += 'k';
    if (m_blackQueenside) castling += 'q';
    if (castling.empty()) castling = "-";
    ss << castling;
    
    // En passant
    ss << ' ';
    if (m_enPassantFile >= 0) {
        ss << static_cast<char>('a' + m_enPassantFile) << (m_sideToMove == Color::White ? '6' : '3');
    } else {
        ss << '-';
    }
    
    // Halfmove and fullmove
    ss << ' ' << m_halfmoveClock << ' ' << m_fullmoveNumber;
    
    return ss.str();
}

const Piece& Board::getPiece(int row, int col) const {
    return m_board[row][col];
}

void Board::setPiece(int row, int col, const Piece& piece) {
    m_board[row][col] = piece;
}

bool Board::makeMove(const Move& move) {
    // Basic move validation and execution
    // This is a simplified implementation
    const Piece& piece = getPiece(move.getFromRow(), move.getFromCol());
    if (piece.isEmpty()) return false;
    
    setPiece(move.getToRow(), move.getToCol(), piece);
    setPiece(move.getFromRow(), move.getFromCol(), Piece());
    
    // Switch side to move
    m_sideToMove = (m_sideToMove == Color::White) ? Color::Black : Color::White;
    
    return true;
}

void Board::undoMove() {
    // TODO: Implement move undo functionality
    // This requires storing move history
}

bool Board::isInCheck(Color color) const {
    // TODO: Implement check detection
    return false;
}

bool Board::isCheckmate(Color color) const {
    // TODO: Implement checkmate detection
    return false;
}

bool Board::isStalemate(Color color) const {
    // TODO: Implement stalemate detection
    return false;
}

} // namespace chess