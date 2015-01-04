#ifndef VECTOR_H
#define VECTOR_H

#define EPSILON 0.0001

#include <cassert>
#include "Matrix.h"

class Vector
{
    public:
        Vector();
        Vector(double x, double y, double z);

        double GetX() const { return x_; }
        double GetY() const { return y_; }
        double GetZ() const { return z_; }

        void SetX(double x) { x_ = x; }
        void SetY(double y) { y_ = y; }
        void SetZ(double z) { z_ = z; }

        Vector operator+(const Vector& right) const;
        Vector operator-(const Vector& right) const;
        Vector operator*(double scalar) const;
        Vector operator-() const;
        Vector Cross(const Vector& right) const;
        Vector Normalize() const;
        double Length() const;
        double Dot(const Vector& right) const;

        Vector Transform(const Matrix& transformationMatrix) const;

        bool operator==(const Vector& right) const;
        friend std::ostream& operator<< (std::ostream& out, const Vector& p);

    private:
        double x_;
        double y_;
        double z_;

        static Matrix pointMatrix;
        static Matrix transformedPointMatrix;
};

#endif
