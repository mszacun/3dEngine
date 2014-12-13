#include <gtest/gtest.h>
#include "../src/Matrix.h"

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

