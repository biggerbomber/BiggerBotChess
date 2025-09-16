#include "utils/utils.h"
#include <sstream>
#include <algorithm>

namespace chess {

std::vector<std::string> Utils::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

std::string Utils::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

bool Utils::squareToCoords(const std::string& square, int& row, int& col) {
    if (square.length() != 2) return false;
    
    char file = square[0];
    char rank = square[1];
    
    if (file < 'a' || file > 'h' || rank < '1' || rank > '8') {
        return false;
    }
    
    col = file - 'a';
    row = 8 - (rank - '0'); // Convert from 1-8 to 7-0 (top to bottom)
    
    return true;
}

std::string Utils::coordsToSquare(int row, int col) {
    if (!isValidCoordinate(row, col)) return "";
    
    char file = 'a' + col;
    char rank = '8' - row; // Convert from 7-0 to 1-8
    
    return std::string(1, file) + std::string(1, rank);
}

bool Utils::isValidCoordinate(int row, int col) {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

} // namespace chess