# BiggerBot Chess Engine Architecture

## Overview

The BiggerBot Chess Engine is designed as a modular, extensible chess engine that follows modern C++ practices and design patterns.

## Core Design Principles

1. **Modularity**: Each component has a clear responsibility and can be developed independently
2. **Extensibility**: Easy to add new features without modifying existing code
3. **Performance**: Designed for efficient chess position evaluation and move search
4. **Maintainability**: Clean code structure with comprehensive documentation

## Component Architecture

### Core Engine (`core/`)
- **Engine**: Main orchestrator that coordinates all components
- Manages game state and provides high-level interface

### Board Representation (`board/`)
- **Board**: 8x8 array-based board representation
- **Piece**: Represents individual chess pieces with type and color
- Handles FEN parsing and board state management

### Move System (`move/`)
- **Move**: Represents chess moves with from/to coordinates
- **MoveGenerator**: Generates legal moves for any position
- Supports algebraic notation conversion

### Search Engine (`search/`)
- **Search**: Implements minimax with alpha-beta pruning
- Extensible for adding new search algorithms
- Quiescence search for tactical positions

### Position Evaluation (`evaluation/`)
- **Evaluator**: Evaluates chess positions
- Material counting, positional factors, king safety
- Easily extensible for advanced evaluation features

### Utilities (`utils/`)
- **Utils**: Common utility functions
- String processing, coordinate conversion
- Board validation helpers

## Data Flow

```
User Input → Engine → Board State Update
                 ↓
           MoveGenerator → Legal Moves
                 ↓
              Search → Best Move Selection
                 ↓
            Evaluator → Position Assessment
```

## Extension Points

### Adding New Search Algorithms
1. Create new class in `src/search/`
2. Implement search interface
3. Update Engine to use new algorithm

### Adding New Evaluation Features
1. Add new evaluation functions to Evaluator
2. Implement in `src/evaluation/`
3. Add weights and tuning parameters

### Adding Communication Protocols
1. Create new protocol handler in `src/protocols/`
2. Implement UCI, XBoard, or custom protocols
3. Integrate with main engine loop

## Build System

The project uses CMake with the following structure:
- Main CMakeLists.txt defines project settings
- Each subdirectory has its own CMakeLists.txt
- Automatic dependency management
- Support for tests and examples

## Future Enhancements

- Opening book integration
- Endgame tablebase support
- Neural network evaluation
- Multi-threading support
- Advanced search techniques (aspiration windows, iterative deepening)
- Time management
- Move ordering improvements