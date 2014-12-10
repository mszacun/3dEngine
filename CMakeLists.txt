project (GrafikaZad3)
cmake_minimum_required (VERSION 2.6)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable (GrafikaZad3 src/main.cpp)
add_executable (Tests tests/TestTests.cpp)
target_link_libraries(Tests ${GTEST_LIBRARIES} pthread)
