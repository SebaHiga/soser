if (${BUILD_BENCHMARKS})

include(FetchContent)

FetchContent_Declare(
  tser
  GIT_REPOSITORY https://github.com/KonanM/tser.git
  GIT_TAG        v1.1
)
FetchContent_MakeAvailable(tser)

add_executable(01BM_Serialization BMSmallSerialization.cpp)
target_compile_options(01BM_Serialization PUBLIC -fconcepts)
target_link_libraries(01BM_Serialization ${CONAN_LIBS} KonanM::tser)
target_include_directories(01BM_Serialization PUBLIC ${CMAKE_SOURCE_DIR}/include/soser)

add_executable(02BM_LargeSerialization BMLargeSerialization.cpp)
target_compile_options(02BM_LargeSerialization PUBLIC -fconcepts)
target_link_libraries(02BM_LargeSerialization SebaHiga::soser ${CONAN_LIBS} KonanM::tser)
target_include_directories(02BM_LargeSerialization PUBLIC ${CMAKE_SOURCE_DIR}/include/soser)

add_executable(03BM_SmallDeserialization BMSmallDeserialization.cpp)
target_compile_options(03BM_SmallDeserialization PUBLIC -fconcepts)
target_link_libraries(03BM_SmallDeserialization SebaHiga::soser ${CONAN_LIBS})
target_include_directories(03BM_SmallDeserialization PUBLIC ${CMAKE_SOURCE_DIR}/include/soser)

endif()
