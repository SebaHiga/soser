# soser

add_library(libsoser 
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/soser.hpp
)

target_include_directories(libsoser
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_sources(libsoser
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/splitting_utils.cpp
)