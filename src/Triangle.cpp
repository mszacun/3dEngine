#include "Triangle.h"

Triangle3D::Triangle3D(const unsigned int& p1, const unsigned  int& p2,
    const unsigned int& p3): p1_(p1), p2_(p2), p3_(p3)
{
}

void Triangle3D::SetTextureCoordinates(const Vector& t1, const Vector& t2, const Vector& t3)
{
    p1TextureCordinates_ = t1;
    p2TextureCordinates_ = t2;
    p3TextureCordinates_ = t3;
}

bool Triangle3D::operator==(const Triangle3D& right) const
{
    return p1_ == right.p1_ && p2_ == right.p2_ && p3_ == right.p3_;
}

Triangle2D::Triangle2D(const Vector& p1, const Vector& p2, const Vector& p3) :
    p1_(p1), p2_(p2), p3_(p3)
{
}

bool Triangle2D::operator==(const Triangle2D right) const
{
    return p1_ == right.p1_ && p2_ == right.p2_ && p3_ == right.p3_;
}
