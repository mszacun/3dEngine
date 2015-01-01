#include "Point.h"

Matrix Point::pointMatrix(1, 4);
Matrix Point::transformedPointMatrix(1, 4);

std::ostream& operator<< (std::ostream& out, const Point& p)
{
    out << "(" << p.x_ << ", " << p.y_ << ", " << p.z_ << ")";
    return out;
}

Point Point::Transform(const Matrix& transformationMatrix) const
{
    pointMatrix.SetElement(0, 0, x_);
    pointMatrix.SetElement(1, 0, y_);
    pointMatrix.SetElement(2, 0, z_);
    pointMatrix.SetElement(3, 0, 1);

    Matrix::Multiply(transformationMatrix, pointMatrix, transformedPointMatrix);

    double w = transformedPointMatrix.GetElement(3, 0);
    return Point(transformedPointMatrix.GetElement(0, 0) / w,
        transformedPointMatrix.GetElement(1, 0) / w,
        transformedPointMatrix.GetElement(2, 0) / w);
}

bool Point::operator==(const Point& right) const
{
    return std::abs(x_ - right.x_) <= EPSILON && 
        std::abs(y_ - right.y_) <= EPSILON && 
        std::abs(z_ - right.z_) <= EPSILON;
}
