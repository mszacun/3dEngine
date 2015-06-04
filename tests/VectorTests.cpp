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

TEST(VectorTests, ShouldCalculateCrossProduct)
{
    Vector v1(3, -3, 1);
    Vector v2(4, 9, 2);
    Vector expectedCrossProduct(-15, -2, 39);

    EXPECT_TRUE(v1.Cross(v2) == expectedCrossProduct);
}

TEST(VectorTests, ShouldCalculateVectorLength)
{
    Vector v(1, 2, 3);

    EXPECT_NEAR(3.742, v.Length(), 0.001);
}

