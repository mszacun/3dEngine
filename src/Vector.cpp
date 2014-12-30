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

bool Vector::operator==(const Vector& right) const
{
    return std::abs(x_ - right.x_) <= EPSILON && 
        std::abs(y_ - right.y_) <= EPSILON && 
        std::abs(z_ - right.z_) <= EPSILON;
}
