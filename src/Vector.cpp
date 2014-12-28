#include "Vector.h"

Vector::Vector(double x, double y, double z): x_(x), y_(y), z_(z)
{
}

Vector::Vector(const Point& start, const Point& end):
    Vector(end.GetX() - start.GetX(), end.GetY() - start.GetY(),
        end.GetZ() - start.GetZ())
{
}

Vector Vector::operator+(const Vector& right) const
{
    return Vector(x_ + right.x_, y_ + right.y_, z_ + right.z_);
}

Vector Vector::operator*(double scalar) const
{
    return Vector(x_ * scalar, y_ * scalar, z_ * scalar);
}

bool Vector::operator==(const Vector& right) const
{
    return std::abs(x_ - right.x_) <= EPSILON && 
        std::abs(y_ - right.y_) <= EPSILON && 
        std::abs(z_ - right.z_) <= EPSILON;
}
