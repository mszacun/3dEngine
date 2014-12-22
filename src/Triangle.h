#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Point.h"

class Triangle3D
{
    public:
        Triangle3D(const unsigned int& p1, const unsigned  int& p2,
            const unsigned int& p3);

        unsigned int GetP1() const { return p1_; }
        unsigned int GetP2() const { return p2_; }
        unsigned int GetP3() const { return p3_; }

    private:
        unsigned int p1_;
        unsigned int p2_;
        unsigned int p3_;
};

struct Triangle2D
{
    Point p1_;
    Point p2_;
    Point p3_;

    Triangle2D(const Point& p1, const Point& p2, const Point& p3);

    bool operator==(const Triangle2D right) const;
};

#endif
