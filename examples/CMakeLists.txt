# Examples
add_executable(simple_game
    simple_game.cpp
)

target_link_libraries(simple_game chess_engine)
target_include_directories(simple_game PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)