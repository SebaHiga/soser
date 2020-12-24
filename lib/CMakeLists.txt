# sopack

add_library(libsopack 
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/sopack.hpp
)

target_include_directories(libsopack
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_sources(libsopack
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/splitting_utils.cpp
)