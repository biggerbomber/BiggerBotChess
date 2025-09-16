# Core chess engine library
add_library(chess_engine
    core/engine.cpp
    board/board.cpp
    board/piece.cpp
    move/move.cpp
    move/move_generator.cpp
    search/search.cpp
    evaluation/evaluator.cpp
    utils/utils.cpp
)

target_include_directories(chess_engine PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# Main executable
add_executable(biggerbot_chess
    main.cpp
)

target_link_libraries(biggerbot_chess chess_engine)