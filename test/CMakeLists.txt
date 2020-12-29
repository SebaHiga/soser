
if(${BUILD_TESTS})

add_library(test-main
    test-main.cpp
)

# CORE Functionalities
add_executable(01T_base core/base.cpp)
target_link_libraries(01T_base ${CONAN_LIBS} test-main libsopack)

add_executable(02T_Deserialization core/deserialization.cpp)
target_link_libraries(02T_Deserialization ${CONAN_LIBS} test-main libsopack)

# UTILS Functionalities
add_executable(03T_Splitters utils/splitters.cpp)
target_link_libraries(03T_Splitters ${CONAN_LIBS} test-main libsopack)


include(CTest)
include(Catch)

catch_discover_tests(01T_base)
catch_discover_tests(02T_Deserialization)
catch_discover_tests(03T_Splitters)

endif()