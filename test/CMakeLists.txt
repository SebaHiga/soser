
if(${BUILD_TESTS})

add_executable(01base base.cpp test-main.cpp)
target_link_libraries(01base ${CONAN_LIBS} libsopack)

add_executable(02Deserialization deserialization.cpp test-main.cpp)
target_link_libraries(02Deserialization ${CONAN_LIBS} libsopack)

include(CTest)
include(Catch)

catch_discover_tests(01base)
catch_discover_tests(02Deserialization)

endif()