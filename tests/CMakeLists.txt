# Simple test framework for the chess engine
add_executable(chess_tests
    test_main.cpp
    test_board.cpp
    test_move.cpp
    test_utils.cpp
)

target_link_libraries(chess_tests chess_engine)
target_include_directories(chess_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Add test discovery
add_test(NAME chess_engine_tests COMMAND chess_tests)