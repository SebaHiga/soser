
if(${BUILD_TESTS})

add_library(test-main
    test-main.cpp
)

# CORE Functionalities
add_executable(01T_base core/base.cpp)
target_link_libraries(01T_base test-main ${CONAN_LIBS})
target_include_directories(01T_base PUBLIC ${CMAKE_SOURCE_DIR}/include/soser)

add_executable(02T_Deserialization core/deserialization.cpp)
target_link_libraries(02T_Deserialization test-main ${CONAN_LIBS})
target_include_directories(02T_Deserialization PUBLIC ${CMAKE_SOURCE_DIR}/include/soser)

add_executable(04T_ContainerSupport core/container_support.cpp)
target_link_libraries(04T_ContainerSupport test-main ${CONAN_LIBS})
target_include_directories(04T_ContainerSupport PUBLIC ${CMAKE_SOURCE_DIR}/include/soser)


# UTILS Functionalities
add_executable(03T_Splitters utils/splitters.cpp)
target_link_libraries(03T_Splitters test-main ${CONAN_LIBS})
target_include_directories(03T_Splitters PUBLIC ${CMAKE_SOURCE_DIR}/include/soser)


include(CTest)
include(Catch)

catch_discover_tests(01T_base)
catch_discover_tests(02T_Deserialization)
catch_discover_tests(03T_Splitters)
catch_discover_tests(04T_ContainerSupport)

endif()