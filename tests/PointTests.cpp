#include <gtest/gtest.h>
#include "../src/Vector.h"

TEST(PointTests, ShouldCreatePoint){
    Vector p(1,666,23);

    EXPECT_EQ(1, p.GetX());
    EXPECT_EQ(666, p.GetY());
    EXPECT_EQ(23, p.GetZ());
}

TEST(PointTests, ShouldScalePoint){
    Vector p(1, 2, 3);

    Matrix transformationMatrix = Matrix::CreateScaleMatrix(2, 0.5, 1);

    Vector result = p.Transform(transformationMatrix);

    EXPECT_EQ(2, result.GetX());
    EXPECT_EQ(1, result.GetY());
    EXPECT_EQ(3, result.GetZ());
}

TEST(PointTests, ShouldTranslatePoint){
    Vector p(1, 2, 3);

    Matrix transformationMatrix = Matrix::CreateTranslationMatrix(0, 20, -5);

    Vector result = p.Transform(transformationMatrix);

    EXPECT_EQ(1, result.GetX());
    EXPECT_EQ(22, result.GetY());
    EXPECT_EQ(-2, result.GetZ());
}

TEST(PointTests, ShouldRotatePointAroundXAxis){
    Vector p(1, 2, 3);

    Matrix transformationMatrix = Matrix::CreateXAxisRotationMatrix(M_PI / 2);

    Vector result = p.Transform(transformationMatrix);

    EXPECT_DOUBLE_EQ(1, result.GetX());
    EXPECT_DOUBLE_EQ(-3, result.GetY());
    EXPECT_DOUBLE_EQ(2, result.GetZ());
}

TEST(PointTests, ShouldRotatePointAroundYAxis){
    Vector p(1, 2, 3);

    Matrix transformationMatrix = Matrix::CreateYAxisRotationMatrix(M_PI / 4);

    Vector result = p.Transform(transformationMatrix);

    EXPECT_DOUBLE_EQ(2 * sqrt(2), result.GetX());
    EXPECT_DOUBLE_EQ(2, result.GetY());
    EXPECT_DOUBLE_EQ(sqrt(2), result.GetZ());
}

TEST(PointTests, ShouldRotatePointAroundZAxis){
    Vector p(1, 2, 3);

    Matrix transformationMatrix = Matrix::CreateZAxisRotationMatrix(M_PI / 6);

    Vector result = p.Transform(transformationMatrix);

    EXPECT_NEAR(-1 + sqrt(3) / 2, result.GetX(), 0.0001); // problems with accuracy
    EXPECT_DOUBLE_EQ(0.5 + sqrt(3), result.GetY());
    EXPECT_DOUBLE_EQ(3, result.GetZ());
}

TEST(PointTests, PointsShouldBeEqualWhenEpsilonIsMet){
    Vector p(1, 2, 3);
    Vector p2(1.0000001, 2.000003, 3.00002);

    EXPECT_TRUE(p == p2);
}

TEST(PointTests, WhenRotated360DegPointShouldStayTheSame)
{
    Vector p(0, 0, -10);
    Vector expectedVector(0, 0, -10);

    Vector rotatedVector = p.Transform(Matrix::CreateXAxisRotationMatrix(M_PI * 2));

    EXPECT_TRUE(expectedVector == rotatedVector);
}
