
if(${BUILD_TESTS})

# CORE Functionalities
add_executable(01base core/base.cpp test-main.cpp)
target_link_libraries(01base ${CONAN_LIBS} libsopack)

add_executable(02Deserialization core/deserialization.cpp test-main.cpp)
target_link_libraries(02Deserialization ${CONAN_LIBS} libsopack)

# UTILS Functionalities
add_executable(03Splitters utils/splitters.cpp test-main.cpp)
target_link_libraries(03Splitters ${CONAN_LIBS} libsopack)


include(CTest)
include(Catch)

catch_discover_tests(01base)
catch_discover_tests(02Deserialization)
catch_discover_tests(03Splitters)

endif()