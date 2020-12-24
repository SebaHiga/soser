
if(${BUILD_TESTS})

add_executable(01base base.cpp test-main.cpp)
target_link_libraries(01base ${CONAN_LIBS} libsopack)

include(CTest)
include(Catch)
catch_discover_tests(01base)

endif()