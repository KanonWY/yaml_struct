cmake_minimum_required(VERSION 3.16)


project(ShowYaml)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(yaml-cpp REQUIRED)

include(${CMAKE_CURRENT_SOURCE_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(parseyaml_test parseyaml_test.cpp)
target_link_libraries(parseyaml_test yaml-cpp pthread)

add_executable(test test.cc)
target_link_libraries(test yaml-cpp ${CONAN_LIBS})
