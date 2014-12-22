#ifndef POINT_H
#define POINT_H

#define EPSILON 0.0001

#include "Matrix.h"
#include <cmath>

class Point
{

    public:
        Point(double x, double y, double z): x_(x), y_(y), z_(z)
        {
        }

        double GetX() const { return x_; }
        double GetY() const { return y_; }
        double GetZ() const { return z_; }

        Point Transform(const Matrix& transformationMatrix) const;

        bool operator==( const Point& right) const;

    private:
        double x_;
        double y_;
        double z_;

        static Matrix pointMatrix;
        static Matrix transformedPointMatrix;
};

#endif
