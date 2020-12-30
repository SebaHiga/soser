
if(${BUILD_TESTS})

add_library(test-main
    test-main.cpp
)

# CORE Functionalities
add_executable(01T_base core/base.cpp)
target_link_libraries(01T_base test-main libsopack ${CONAN_LIBS})

add_executable(02T_Deserialization core/deserialization.cpp)
target_link_libraries(02T_Deserialization test-main libsopack ${CONAN_LIBS})

# UTILS Functionalities
add_executable(03T_Splitters utils/splitters.cpp)
target_link_libraries(03T_Splitters test-main libsopack ${CONAN_LIBS})


include(CTest)
include(Catch)

catch_discover_tests(01T_base)
catch_discover_tests(02T_Deserialization)
catch_discover_tests(03T_Splitters)

endif()