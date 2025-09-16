
cmake_minimum_required(VERSION 3.10)
project(BiggerBotChess)

set(CMAKE_CXX_STANDARD 17)

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

# Collect all .cpp files in src/
file(GLOB SRC_FILES src/*.cpp)

add_executable(BiggerBotChess ${SRC_FILES})
