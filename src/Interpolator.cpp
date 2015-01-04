#include "Interpolator.h"

Matrix pMatrix(1, 3);
Matrix lambdas (1, 3);
Matrix conversionMatrix(3, 3);

BarycentricCoordinates ConvertToBarycentric(const Vector& p1, const Vector& p2,
    const Vector& p3, const Vector& p)
{
    BarycentricCoordinates result;

    Matrix::Set3x3Matrix(conversionMatrix,
            p1.GetX(), p2.GetX(), p3.GetX(),
            p1.GetY(), p2.GetY(), p3.GetY(),
            p1.GetZ(), p2.GetZ(), p3.GetZ());

    pMatrix.SetElement(0, 0, p.GetX());
    pMatrix.SetElement(1, 0, p.GetY());
    pMatrix.SetElement(2, 0, p.GetZ());

    Matrix::Multiply(conversionMatrix.Invert3x3Matrix(), pMatrix, lambdas);

    result.l1 = lambdas.GetElement(0, 0);
    result.l2 = lambdas.GetElement(1, 0);
    result.l3 = lambdas.GetElement(2, 0);

    return result;
}
