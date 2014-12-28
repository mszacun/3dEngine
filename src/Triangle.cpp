#include "Triangle.h"

Triangle3D::Triangle3D(const unsigned int& p1, const unsigned  int& p2,
    const unsigned int& p3): p1_(p1), p2_(p2), p3_(p3)
{
}

bool Triangle3D::operator==(const Triangle3D& right) const
{
    return p1_ == right.p1_ && p2_ == right.p2_ && p3_ == right.p3_;
}

Triangle2D::Triangle2D(const Point& p1, const Point& p2, const Point& p3) :
    p1_(p1), p2_(p2), p3_(p3)
{
}

bool Triangle2D::operator==(const Triangle2D right) const
{
    return p1_ == right.p1_ && p2_ == right.p2_ && p3_ == right.p3_;
}
