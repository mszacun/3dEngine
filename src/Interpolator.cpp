#include "Interpolator.h"

BarycentricCoordinates ConvertToBarycentric(const Point& p1, const Point& p2,
    const Point& p3, const Point& p)
{
    BarycentricCoordinates result;

    std::vector<std::vector<double>> matrixData
    {
        { p1.GetX(), p2.GetX(), p3.GetX() },
        { p1.GetY(), p2.GetY(), p3.GetY() },
        { p1.GetZ(), p2.GetZ(), p3.GetZ() }
    };
    std::vector<std::vector<double>> pData { { p.GetX() }, { p.GetY() }, { p.GetZ() } };

    Matrix conversionMatrix(matrixData);
    Matrix pMatrix(pData);

    Matrix lambdas (1, 3);
    Matrix::Multiply(conversionMatrix.Invert3x3Matrix(), pMatrix, lambdas);

    result.l1 = lambdas.GetElement(0, 0);
    result.l2 = lambdas.GetElement(1, 0);
    result.l3 = lambdas.GetElement(2, 0);

    return result;
}
