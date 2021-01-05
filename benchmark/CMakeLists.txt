if (${BUILD_BENCHMARKS})

include(FetchContent)

FetchContent_Declare(
  tser
  GIT_REPOSITORY https://github.com/KonanM/tser.git
  GIT_TAG        v1.1
)
FetchContent_MakeAvailable(tser)

add_executable(01BM_Serialization BMSmallSerialization.cpp)
target_link_libraries(01BM_Serialization ${CONAN_LIBS} libsopack KonanM::tser)

add_executable(02BM_LargeSerialization BMLargeSerialization.cpp)
target_link_libraries(02BM_LargeSerialization ${CONAN_LIBS} libsopack KonanM::tser)

add_executable(03BM_SmallDeserialization BMSmallDeserialization.cpp)
target_link_libraries(03BM_SmallDeserialization ${CONAN_LIBS} libsopack)

endif()