# BiggerBotChess

A modular and extensible C++ chess engine built with CMake, designed for easy development and expansion.

## Features

- **Modular Architecture**: Well-organized codebase with separate modules for different engine components
- **CMake Build System**: Cross-platform build configuration with proper dependency management
- **VS Code Integration**: Complete IDE setup with build, run, and debug configurations
- **Extensible Design**: Easy to add new features, algorithms, and evaluation functions
- **Modern C++17**: Uses modern C++ features and best practices

## Project Structure

```
├── src/                    # Source code
│   ├── core/              # Core engine logic
│   ├── board/             # Board representation and piece management
│   ├── move/              # Move generation and validation
│   ├── search/            # Search algorithms (minimax, alpha-beta)
│   ├── evaluation/        # Position evaluation functions
│   ├── utils/             # Utility functions
│   └── main.cpp           # Main executable
├── include/               # Header files (mirroring src structure)
├── tests/                 # Unit tests
├── examples/              # Example programs
├── docs/                  # Documentation
├── .vscode/               # VS Code configuration
└── CMakeLists.txt         # Main CMake configuration
```

## Building the Project

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 6+, MSVC 2017+)
- CMake 3.16 or higher
- Git

### Command Line Build

```bash
# Clone the repository
git clone https://github.com/biggerbomber/BiggerBotChess.git
cd BiggerBotChess

# Configure and build
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4

# Run the chess engine
./build/src/biggerbot_chess
```

### VS Code Build

1. Open the project in VS Code
2. Install the recommended extensions:
   - C/C++ Extension Pack
   - CMake Tools
3. Press `Ctrl+Shift+P` and run "CMake: Configure"
4. Press `F7` to build or use `Ctrl+Shift+P` → "Tasks: Run Build Task"
5. Press `F5` to run with debugging

## VS Code Integration

The project includes complete VS Code configuration:

- **Build Tasks** (`Ctrl+Shift+P` → "Tasks: Run Task"):
  - `configure`: Configure CMake project
  - `build`: Build the project (default build task - `Ctrl+Shift+B`)
  - `build-release`: Build release version
  - `clean`: Clean build artifacts
  - `run`: Build and run the chess engine

- **Debug Configuration** (`F5`):
  - `Debug Chess Engine`: Debug the main chess engine
  - `Debug Unit Tests`: Debug the test suite

- **IntelliSense**: Full code completion and navigation support

## Running Tests

```bash
# Build and run tests
cmake --build build
cd build
ctest

# Or run tests directly
./tests/chess_tests
```

## Usage Examples

### Basic Engine Usage

```cpp
#include "core/engine.h"

int main() {
    chess::Engine engine;
    engine.initialize();
    
    // Make moves
    engine.makeMove("e2e4");
    engine.makeMove("e7e5");
    
    // Find best move
    std::string bestMove = engine.findBestMove(6);
    std::cout << "Best move: " << bestMove << std::endl;
    
    return 0;
}
```

### Interactive Mode

Run the main executable for an interactive chess session:

```bash
./build/src/biggerbot_chess
```

Commands:
- `move <move>`: Make a move (e.g., "move e2e4")
- `search`: Find the best move
- `position`: Show current position in FEN notation
- `quit`: Exit

## Architecture Overview

### Core Components

1. **Engine** (`core/engine.h`): Main orchestrator that coordinates all components
2. **Board** (`board/board.h`): Represents the chess board and game state
3. **Piece** (`board/piece.h`): Represents individual chess pieces
4. **Move** (`move/move.h`): Represents chess moves with conversion utilities
5. **MoveGenerator** (`move/move_generator.h`): Generates legal moves
6. **Search** (`search/search.h`): Implements search algorithms
7. **Evaluator** (`evaluation/evaluator.h`): Evaluates board positions
8. **Utils** (`utils/utils.h`): Utility functions and helpers

### Extensibility Points

The engine is designed for easy extension:

- **Search Algorithms**: Add new search methods in `src/search/`
- **Evaluation Functions**: Extend position evaluation in `src/evaluation/`
- **Move Generation**: Enhance move generation in `src/move/`
- **Board Representations**: Alternative board representations in `src/board/`
- **Protocols**: Add UCI/XBoard protocols in `src/protocols/`

## Development Guidelines

### Adding New Features

1. Create appropriate header files in `include/`
2. Implement functionality in `src/`
3. Add tests in `tests/`
4. Update CMakeLists.txt if needed
5. Document the new features

### Code Style

- Use modern C++17 features
- Follow RAII principles
- Use `snake_case` for functions and variables
- Use `PascalCase` for classes and types
- Include comprehensive documentation

### Testing

- Write unit tests for new functionality
- Use the simple test framework provided
- Run tests before committing changes

## Contributing

1. Fork the repository
2. Create a feature branch
3. Implement your changes with tests
4. Ensure all tests pass
5. Submit a pull request

## Roadmap

- [ ] Complete move generation for all pieces
- [ ] Implement full check/checkmate detection
- [ ] Add opening book support
- [ ] Implement UCI protocol
- [ ] Add endgame tablebase support
- [ ] Performance optimizations
- [ ] Advanced evaluation features
- [ ] Machine learning evaluation

## License

This project is open source. See LICENSE file for details.

## Acknowledgments

Built with modern C++ and CMake for maximum portability and extensibility.