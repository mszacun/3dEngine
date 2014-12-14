#include <gtest/gtest.h>
#include "../src/Point.h"

TEST(PointTests, ShouldCreatePoint){
    Point p(1,666,23);

    EXPECT_EQ(1, p.GetX());
    EXPECT_EQ(666, p.GetY());
    EXPECT_EQ(23, p.GetZ());
}

TEST(PointTests, ShouldScalePoint){
    Point p(1, 2, 3);

    Matrix transformationMatrix = Matrix::CreateScaleMatrix(2, 0.5, 1);

    Point result = p.Transform(transformationMatrix);

    EXPECT_EQ(2, result.GetX());
    EXPECT_EQ(1, result.GetY());
    EXPECT_EQ(3, result.GetZ());
}

TEST(PointTests, ShouldTranslatePoint){
    Point p(1, 2, 3);

    Matrix transformationMatrix = Matrix::CreateTranslationMatrix(0, 20, -5);

    Point result = p.Transform(transformationMatrix);

    EXPECT_EQ(1, result.GetX());
    EXPECT_EQ(22, result.GetY());
    EXPECT_EQ(-2, result.GetZ());
}

TEST(PointTests, ShouldRotatePointAroundXAxis){
    Point p(1, 2, 3);

    Matrix transformationMatrix = Matrix::CreateXAxisRotationMatrix(90);

    Point result = p.Transform(transformationMatrix);

    EXPECT_DOUBLE_EQ(1, result.GetX());
    EXPECT_DOUBLE_EQ(-3, result.GetY());
    EXPECT_DOUBLE_EQ(2, result.GetZ());
}

TEST(PointTests, ShouldRotatePointAroundYAxis){
    Point p(1, 2, 3);

    Matrix transformationMatrix = Matrix::CreateYAxisRotationMatrix(45);

    Point result = p.Transform(transformationMatrix);

    EXPECT_DOUBLE_EQ(2 * sqrt(2), result.GetX());
    EXPECT_DOUBLE_EQ(2, result.GetY());
    EXPECT_DOUBLE_EQ(sqrt(2), result.GetZ());
}

TEST(PointTests, ShouldRotatePointAroundZAxis){
    Point p(1, 2, 3);

    Matrix transformationMatrix = Matrix::CreateZAxisRotationMatrix(30);

    Point result = p.Transform(transformationMatrix);

    EXPECT_NEAR(-1 + sqrt(3) / 2, result.GetX(), 0.0001); // problems with accuracy
    EXPECT_DOUBLE_EQ(0.5 + sqrt(3), result.GetY());
    EXPECT_DOUBLE_EQ(3, result.GetZ());
}
