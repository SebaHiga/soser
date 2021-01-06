cmake_minimum_required(VERSION 3.16.3)
project(soserializer)

# Basic C++ settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconcepts -Wall -Wextra -Werror -O3")

# Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

#libsoser
add_subdirectory("lib")

#benchmarks
add_subdirectory("benchmark")


#Executables and tests
add_executable(soser main.cpp)

enable_testing()
add_subdirectory(test)

#Main target settings
target_include_directories(soser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/inc
)

target_link_libraries(soser
    PRIVATE
        libsoser
)