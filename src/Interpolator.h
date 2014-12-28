#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include "Point.h"

struct BarycentricCoordinates
{
    double l1;
    double l2;
    double l3;
};

BarycentricCoordinates ConvertToBarycentric(const Point& p1, const Point& p2,
    const Point& p3, const Point& p);

template <typename T>
class Triangle3DInterpolator
{
    public:
        Triangle3DInterpolator();

        void SetPoint1(const Point& p, const T& value);
        void SetPoint2(const Point& p, const T& value);
        void SetPoint3(const Point& p, const T& value);

        T Interpolate(const Point& p);

    private:
        Point p1_;
        T p1Value_;

        Point p2_;
        T p2Value_;

        Point p3_;
        T p3value;
};

#endif
