#ifndef VECTOR_H
#define VECTOR_H

#include "Point.h"

class Vector
{
    public:
        Vector(double x, double y, double z);
        Vector(const Point& start, const Point& end);

        Vector operator+(const Vector& right) const;
        Vector operator*(double scalar) const;

        bool operator==(const Vector& right) const;

    private:
        double x_;
        double y_;
        double z_;
};

#endif
