project (GrafikaZad3)
cmake_minimum_required (VERSION 2.6)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable (GrafikaZad3 src/main.cpp)
add_executable (UT tests/TestTests.cpp tests/PointTests.cpp)
target_link_libraries(UT ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
add_test(ut, UT)
