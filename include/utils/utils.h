#pragma once

#include <string>
#include <vector>

namespace chess {

/**
 * @brief Utility functions for the chess engine
 */
class Utils {
public:
    /**
     * @brief Split a string by delimiter
     * @param str String to split
     * @param delimiter Delimiter character
     * @return Vector of split strings
     */
    static std::vector<std::string> split(const std::string& str, char delimiter);
    
    /**
     * @brief Trim whitespace from string
     * @param str String to trim
     * @return Trimmed string
     */
    static std::string trim(const std::string& str);
    
    /**
     * @brief Convert square notation to row/col coordinates
     * @param square Square in algebraic notation (e.g., "e4")
     * @param row Output row (0-7)
     * @param col Output column (0-7)
     * @return True if valid square notation
     */
    static bool squareToCoords(const std::string& square, int& row, int& col);
    
    /**
     * @brief Convert row/col coordinates to square notation
     * @param row Row (0-7)
     * @param col Column (0-7)
     * @return Square in algebraic notation
     */
    static std::string coordsToSquare(int row, int col);
    
    /**
     * @brief Check if coordinates are within board bounds
     * @param row Row coordinate
     * @param col Column coordinate
     * @return True if valid coordinates
     */
    static bool isValidCoordinate(int row, int col);
};

} // namespace chess