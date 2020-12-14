cmake_minimum_required(VERSION 3.16.3)

project(sopacker)

set(CMAKE_CXX_STANDARD 17)

add_executable(sop main.cpp)

target_include_directories(sop PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_compile_options(-Wall -Wextra -pedantic -Werror -O3)