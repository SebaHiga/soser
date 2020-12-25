
if(${BUILD_TESTS})

add_library(test-main
    test-main.cpp
)

# CORE Functionalities
add_executable(01base core/base.cpp)
target_link_libraries(01base ${CONAN_LIBS} test-main libsopack)

add_executable(02Deserialization core/deserialization.cpp)
target_link_libraries(02Deserialization ${CONAN_LIBS} test-main libsopack)

# UTILS Functionalities
add_executable(03Splitters utils/splitters.cpp)
target_link_libraries(03Splitters ${CONAN_LIBS} test-main libsopack)


include(CTest)
include(Catch)

catch_discover_tests(01base)
catch_discover_tests(02Deserialization)
catch_discover_tests(03Splitters)

endif()