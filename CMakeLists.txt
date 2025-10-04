
cmake_minimum_required(VERSION 3.10)
project(BiggerBotChess)

set(CMAKE_CXX_STANDARD 17)
#Add all waring flags
if(MSVC)
	add_compile_options(/W4 /WX)
else()
	add_compile_options(-Wall -Wextra -Werror)
endif()
# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

# Collect all .cpp files in src/
file(GLOB SRC_FILES src/*.cpp)

add_executable(BiggerBotChess ${SRC_FILES})
