#ifndef VECTOR_H
#define VECTOR_H

#include "Point.h"
#include <cassert>

class Vector
{
    public:
        Vector();
        Vector(double x, double y, double z);
        Vector(const Point& start, const Point& end);

        double GetX() const { return x_; }
        double GetY() const { return y_; }
        double GetZ() const { return z_; }

        Vector operator+(const Vector& right) const;
        Vector operator-(const Vector& right) const;
        Vector operator*(double scalar) const;
        Vector operator-() const;
        Vector Cross(const Vector& right) const;
        Vector Normalize() const;
        double Length() const;
        double Dot(const Vector& right) const;

        bool operator==(const Vector& right) const;

    private:
        double x_;
        double y_;
        double z_;
};

#endif
