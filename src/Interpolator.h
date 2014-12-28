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
        Triangle3DInterpolator(const Point p1, const T& value1,
            const Point p2, const T& value2,
            const Point p3, const T& value3);

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
        T p3Value_;
};

template <typename T>
Triangle3DInterpolator<T>::Triangle3DInterpolator(const Point p1, const T& value1,
    const Point p2, const T& value2, const Point p3, const T& value3):
    p1_(p1), p1Value_(value1), p2_(p2), p2Value_(value2), p3_(p3), p3Value_(value3)
{
}

template <typename T>
void Triangle3DInterpolator<T>::SetPoint1(const Point& p, const T& value)
{
    p1_ = p;
    p1Value_ = value;
}

template <typename T>
void Triangle3DInterpolator<T>::SetPoint2(const Point& p, const T& value)
{
    p2_ = p;
    p2Value_ = value;
}

template <typename T>
void Triangle3DInterpolator<T>::SetPoint3(const Point& p, const T& value)
{
    p3_ = p;
    p3Value_ = value;
}

template <typename T>
T Triangle3DInterpolator<T>::Interpolate(const Point& p)
{
    BarycentricCoordinates barycentric = ConvertToBarycentric(p1_, p2_, p3_, p);

    return barycentric.l1 * p1Value_ + barycentric.l2 * p2Value_ + barycentric.l3 * p3Value_;
}

#endif
