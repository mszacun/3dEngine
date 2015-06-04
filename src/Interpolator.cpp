#include "Interpolator.h"

Matrix pMatrix(1, 3);
Matrix lambdas (1, 3);
Matrix conversionMatrix(3, 3);

BarycentricCoordinates ConvertToBarycentric(const Vector& a, const Vector& b,
    const Vector& c, const Vector& vec)
{
    BarycentricCoordinates result;
    double den = 1.0 / ((b.GetY() - c.GetY()) * (a.GetX() - c.GetX()) + (c.GetX() - b.GetX()) * (a.GetY() - c.GetY()));

    result.l1 = ((b.GetY() - c.GetY()) * (vec.GetX() - c.GetX()) + (c.GetX() - b.GetX()) * (vec.GetY() - c.GetY())) * den;
    result.l2 = ((c.GetY() - a.GetY()) * (vec.GetX() - c.GetX()) + (a.GetX() - c.GetX()) * (vec.GetY() - c.GetY())) * den;
    result.l3 = 1 - result.l1 - result.l2;

    return result;
}
