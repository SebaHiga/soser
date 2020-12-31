if (${BUILD_BENCHMARKS})

add_executable(01BM_Serialization BMSmallSerialization.cpp)
target_link_libraries(01BM_Serialization ${CONAN_LIBS} libsopack)

add_executable(02BM_LargeSerialization BMLargeSerialization.cpp)
target_link_libraries(02BM_LargeSerialization ${CONAN_LIBS} libsopack)

endif()