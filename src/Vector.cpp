#include "Vector.h"

Matrix Vector::pointMatrix(1, 4);
Matrix Vector::transformedPointMatrix(1, 4);

Vector::Vector() : Vector(0, 0, 0)
{
}

Vector::Vector(double x, double y, double z): x_(x), y_(y), z_(z)
{
}

Vector Vector::operator+(const Vector& right) const
{
    return Vector(x_ + right.x_, y_ + right.y_, z_ + right.z_);
}

Vector Vector::operator-(const Vector& right) const
{
    return Vector(x_ - right.x_, y_ - right.y_, z_ - right.z_);
}

Vector Vector::operator*(double scalar) const
{
    return Vector(x_ * scalar, y_ * scalar, z_ * scalar);
}

Vector Vector::operator-() const
{
    return Vector(-x_, -y_, -z_);
}

Vector Vector::Cross(const Vector& right) const
{
    double i = y_ * right.z_ - z_ * right.y_;
    double j = z_ * right.x_ - x_ * right.z_;
    double k = x_ * right.y_ - y_ * right.x_;

    return Vector(i, j, k);
}

Vector Vector::Normalize() const
{
    double length = Length();

    return Vector(x_ / length, y_ / length, z_ / length);
}

double Vector::Length() const
{
    return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
}

double Vector::Dot(const Vector& right) const
{
    return x_ * right.x_ + y_ * right.y_ + z_ * right.z_;
}

Vector Vector::Transform(const Matrix& transformationMatrix) const
{
    pointMatrix.SetElement(0, 0, x_);
    pointMatrix.SetElement(1, 0, y_);
    pointMatrix.SetElement(2, 0, z_);
    pointMatrix.SetElement(3, 0, 1);

    Matrix::Multiply(transformationMatrix, pointMatrix, transformedPointMatrix);

    double w = transformedPointMatrix.GetElement(3, 0);
    return Vector(transformedPointMatrix.GetElement(0, 0) / w,
        transformedPointMatrix.GetElement(1, 0) / w,
        transformedPointMatrix.GetElement(2, 0) / w);
}

bool Vector::operator==(const Vector& right) const
{
    return std::abs(x_ - right.x_) <= EPSILON && 
        std::abs(y_ - right.y_) <= EPSILON && 
        std::abs(z_ - right.z_) <= EPSILON;
}

std::ostream& operator<< (std::ostream& out, const Vector& v)
{
    out << "(" << v.x_ << ", " << v.y_ << ", " << v.z_ << ")";
    return out;
}
