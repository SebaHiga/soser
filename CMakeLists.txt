cmake_minimum_required(VERSION 3.13.4)
project(soser)

add_library(soser INTERFACE)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -O3 -std=c++17")

# Conan
if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
endif()

#benchmarks
add_subdirectory("benchmark")

enable_testing()
add_subdirectory(test)

#Interface target settings
target_include_directories(soser
    INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

# target_compile_features(soser INTERFACE cxx_std_17)
target_compile_options(soser INTERFACE -std=c++2a)

if (${BUILD_SAMPLE})
#Executables and tests
add_executable(sample main.cpp)
target_include_directories(sample PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(sample PRIVATE soser)
endif()
