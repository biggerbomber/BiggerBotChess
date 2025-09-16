#!/bin/bash

# BiggerBot Chess Engine Build Script
echo "BiggerBot Chess Engine Build Script"
echo "==================================="

# Check if CMake is installed
if ! command -v cmake &> /dev/null; then
    echo "Error: CMake is not installed. Please install CMake 3.16 or higher."
    exit 1
fi

# Check CMake version
CMAKE_VERSION=$(cmake --version | head -n1 | awk '{print $3}')
echo "Found CMake version: $CMAKE_VERSION"

# Create build directory
BUILD_DIR="build"
BUILD_TYPE=${1:-"Release"}

echo "Build type: $BUILD_TYPE"
echo "Build directory: $BUILD_DIR"

# Configure
echo "Configuring project..."
cmake -B $BUILD_DIR -S . \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DBUILD_TESTS=ON \
    -DBUILD_EXAMPLES=ON

if [ $? -ne 0 ]; then
    echo "Error: CMake configuration failed!"
    exit 1
fi

# Build
echo "Building project..."
cmake --build $BUILD_DIR -j$(nproc 2>/dev/null || echo 4)

if [ $? -ne 0 ]; then
    echo "Error: Build failed!"
    exit 1
fi

echo ""
echo "Build completed successfully!"
echo ""
echo "To run the chess engine:"
echo "  ./$BUILD_DIR/src/biggerbot_chess"
echo ""
echo "To run tests:"
echo "  ./$BUILD_DIR/tests/chess_tests"
echo ""
echo "To run examples:"
echo "  ./$BUILD_DIR/examples/simple_game"