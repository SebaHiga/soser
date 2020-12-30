cmake_minimum_required(VERSION 3.16.3)
project(sopacker)

# Basic C++ settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconcepts -Wall -Wextra -Werror -O3")

# Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

#libsopack
add_subdirectory("lib")

#benchmarks
add_subdirectory("benchmark")


#Executables and tests
add_executable(sop main.cpp)

enable_testing()
add_subdirectory(test)

#Main target settings
target_include_directories(sop PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/inc
)

target_link_libraries(sop
    PRIVATE
        libsopack
)