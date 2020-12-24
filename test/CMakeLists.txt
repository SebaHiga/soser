enable_testing()

add_executable(01base base.cpp test-main.cpp)
target_include_directories(01base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(01base ${CONAN_LIBS} libsopack)

include(CTest)
include(Catch)
catch_discover_tests(01base)