#include <gtest/gtest.h>
#include "../src/Matrix.h"
#include "../src/Vector.h"

using std::vector;

TEST(MatrixTests, ShouldCreateMatrix){
    vector<vector<double>> m1Data { vector<double> { 1, 2, 3 },
                            vector<double> { 4, 5, 6 },
                            vector<double> { 7, 8, 9 }
                          };
    Matrix m(m1Data);

    for (unsigned int i = 0; i < m1Data.size(); i++)
        for (unsigned int j = 0; j < m1Data[i].size(); j++)
            EXPECT_EQ(m1Data[i][j], m.GetElement(i, j));
}

TEST(MatrixTests, ShouldCheckMatrixEquality)
{
    vector<vector<double>> m1Data { vector<double> { 1, 2, 3 },
                            vector<double> { 4, 5, 6 },
                            vector<double> { 7, 8, 9 }
                          };
    Matrix m1(m1Data);
    Matrix m2(m1Data);

    EXPECT_TRUE(m1 == m2);
}

TEST(MatrixTests, ShouldMultipleTwoMatrixes){
    vector<vector<double>> m1Data { vector<double> { 1, 2, 3 },
                            vector<double> { 4, 5, 6 },
                            vector<double> { 7, 8, 9 }
                          };
    vector<vector<double>> m2Data { vector<double> { 1 },
                            vector<double> { 1 },
                            vector<double> { 1 }
                          };
    vector<vector<double>> resultData { vector<double> { 6 },
                                        vector<double> { 15 },
                                        vector<double> { 24 }
                                      };
    Matrix m1(m1Data);
    Matrix m2(m2Data);
    Matrix expectedResult(resultData);
    Matrix result(1, 3);

    Matrix::Multiply(m1, m2, result);

    EXPECT_TRUE(expectedResult == result);
}

TEST(MatrixTests, ShouldSubtractTwoMatrixes){
    vector<vector<double>> m1Data {
                            { 1, 2, 3 },
                            { 4, 5, 6 },
                            { 7, 8, 9 }
                          };
    vector<vector<double>> m2Data {
                            { 2, 5, 8 },
                            { 1, -2, 0 },
                            { 4, 0, -3 }
                          };
    vector<vector<double>> resultData {
                                        { -1, -3, -5 },
                                        { 3, 7, 6 },
                                        { 3, 8, 12 }
                                      };
    Matrix m1(m1Data);
    Matrix m2(m2Data);
    Matrix expectedResult(resultData);
    Matrix result = m1 - m2;

    EXPECT_TRUE(expectedResult == result);
}

TEST(MatrixTests, ShouldInvertMatrix)
{
    vector<vector<double>> matrixData { { 1, 2, 3 }, { 0, 1, 4 }, { 5, 6, 0 } };
    vector<vector<double>> expectedData{ { -24, 18, 5 }, { 20, -15, -4 }, { -5, 4, 1 } };

    Matrix original(matrixData);
    Matrix expectedInverted(expectedData);

    Matrix invertedMatrix = original.Invert3x3Matrix();

    EXPECT_TRUE(expectedInverted ==  invertedMatrix);
}

TEST(MatrixTests, ShouldCalculatePerspectiveProjectionMatrix)
{
    Matrix projectionMatrix = Matrix::CreatePerspectiveProjectionMatrix2(0.78, 1, 0.01, 1.0);

    EXPECT_NEAR(2.432765, projectionMatrix.GetElement(0, 0), 0.000001);
    EXPECT_NEAR(0, projectionMatrix.GetElement(0, 1), 0.000001);
    EXPECT_NEAR(0, projectionMatrix.GetElement(0, 2), 0.000001);
    EXPECT_NEAR(0, projectionMatrix.GetElement(0, 3), 0.000001);

    EXPECT_NEAR(0, projectionMatrix.GetElement(1, 0), 0.000001);
    EXPECT_NEAR(2.432765, projectionMatrix.GetElement(1, 1), 0.000001);
    EXPECT_NEAR(0, projectionMatrix.GetElement(1, 2), 0.000001);
    EXPECT_NEAR(0, projectionMatrix.GetElement(1, 3), 0.000001);

    EXPECT_NEAR(0, projectionMatrix.GetElement(2, 0), 0.000001);
    EXPECT_NEAR(0, projectionMatrix.GetElement(2, 1), 0.000001);
    EXPECT_NEAR(-1.010101, projectionMatrix.GetElement(2, 2), 0.000001);
    EXPECT_NEAR(-1, projectionMatrix.GetElement(2, 3), 0.000001);

    EXPECT_NEAR(0, projectionMatrix.GetElement(3, 0), 0.000001);
    EXPECT_NEAR(0, projectionMatrix.GetElement(3, 1), 0.000001);
    EXPECT_NEAR(-0.01010101, projectionMatrix.GetElement(3, 2), 0.000001);
    EXPECT_NEAR(0, projectionMatrix.GetElement(3, 3), 0.000001);
}

TEST(MatrixTests, ShouldCalculateViewMatrix)
{
    Matrix viewMatrix = Matrix::CreateViewMatrix(Vector(0, 0, -10), Vector(0, 0, 0), Vector(0, 1, 0));

    EXPECT_NEAR(-1, viewMatrix.GetElement(0, 0), 0.000001);
    EXPECT_NEAR(0, viewMatrix.GetElement(0, 1), 0.000001);
    EXPECT_NEAR(0, viewMatrix.GetElement(0, 2), 0.000001);
    EXPECT_NEAR(0, viewMatrix.GetElement(0, 3), 0.000001);

    EXPECT_NEAR(0, viewMatrix.GetElement(1, 0), 0.000001);
    EXPECT_NEAR(1, viewMatrix.GetElement(1, 1), 0.000001);
    EXPECT_NEAR(0, viewMatrix.GetElement(1, 2), 0.000001);
    EXPECT_NEAR(0, viewMatrix.GetElement(1, 3), 0.000001);

    EXPECT_NEAR(0, viewMatrix.GetElement(2, 0), 0.000001);
    EXPECT_NEAR(0, viewMatrix.GetElement(2, 1), 0.000001);
    EXPECT_NEAR(-1, viewMatrix.GetElement(2, 2), 0.000001);
    EXPECT_NEAR(0, viewMatrix.GetElement(2, 3), 0.000001);

    EXPECT_NEAR(0, viewMatrix.GetElement(3, 0), 0.000001);
    EXPECT_NEAR(0, viewMatrix.GetElement(3, 1), 0.000001);
    EXPECT_NEAR(10, viewMatrix.GetElement(3, 2), 0.000001);
    EXPECT_NEAR(1, viewMatrix.GetElement(3, 3), 0.000001);
}
