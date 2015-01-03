project (GrafikaZad3)
cmake_minimum_required (VERSION 2.6)
find_package(GTest REQUIRED)
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})
include_directories(${GTEST_INCLUDE_DIRS})
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -ggdb")
set(MOC_REQUIRED_HEADERS src/View.h)
QT4_WRAP_CPP(HEADERS_MOCED ${MOC_REQUIRED_HEADERS})

add_library(GrafikaZad3Lib src/Scene.cpp src/Triangle.cpp src/Matrix.cpp
    src/Interpolator.cpp src/Vector.cpp src/ObjDeserializer.cpp
    src/FlatShader.cpp)

add_executable (GrafikaZad3 src/main.cpp src/View.cpp src/Controler.cpp ${HEADERS_MOCED})
target_link_libraries(GrafikaZad3 ${QT_LIBRARIES} GrafikaZad3Lib)

add_executable (UT tests/TestTests.cpp tests/PointTests.cpp tests/SceneTests.cpp
    tests/MatrixTests tests/InterpolatorTests.cpp tests/VectorTests.cpp
    tests/ObjDeserializerTests.cpp)
target_link_libraries(UT ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} ${QT_LIBRARIES} pthread GrafikaZad3Lib)
add_test(ut, UT)

add_executable(BENCHMARK tests/Benchmark.cpp)
target_link_libraries(BENCHMARK GrafikaZad3Lib)
add_test(benchmark, BENCHMARK)

