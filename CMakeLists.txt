cmake_minimum_required(VERSION 3.13.4)
project(soserializer)

# Conan
if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
endif()

#benchmarks
add_subdirectory("benchmark")

#Executables and tests
add_executable(soser main.cpp)

enable_testing()
add_subdirectory(test)

#Main target settings
target_include_directories(soser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/soser
)

# Basic C++ settings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconcepts -Wall -Wextra -Werror -O3 -std=c++17")