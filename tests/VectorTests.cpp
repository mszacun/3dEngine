#include <gtest/gtest.h>
#include "../src/Vector.h"

TEST(VectorTests, ShouldCreateVector){
    Vector v(1, 2,3);
}

TEST(VectorTests, CheckTwoVectorsEqulity){
    Vector v(1, 2,3);
    Vector v2(1.00000001, 2, 3);

    EXPECT_TRUE(v == v2);
}

TEST(VectorTests, ShouldCreateVectorFromTwoPoints){
    Point start(4, -2, 0);
    Point end(1, 5, -10);
    Vector expectedVector(-3, 7, -10);

    Vector calculatedVector(start, end);

    EXPECT_TRUE(expectedVector == calculatedVector);
}

TEST(VectorTests, ShouldAddTwoVectors){
    Vector v(1, 2,3);
    Vector v2(-6, 12, 5);
    Vector expectedVector(-5, 14, 8);

    EXPECT_TRUE(v + v2 == expectedVector);
}

TEST(VectorTests, ShouldMultiplyVectorTimesScalar){
    Vector v(1, 2,3);
    double scalar = 2.5;
    Vector expectedVector(2.5, 5, 7.5);

    EXPECT_TRUE(v * scalar == expectedVector);
}
