#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vector.h"

class Triangle3D
{
    public:
        Triangle3D(const unsigned int& p1, const unsigned  int& p2,
            const unsigned int& p3);

        unsigned int GetP1() const { return p1_; }
        unsigned int GetP2() const { return p2_; }
        unsigned int GetP3() const { return p3_; }

        const Vector& GetP1TextureCoordinates() const { return p1TextureCordinates_; }
        const Vector& GetP2TextureCoordinates() const  { return p2TextureCordinates_; }
        const Vector& GetP3TextureCoordinates() const { return p3TextureCordinates_; }

        void SetNormal(const Vector& normal) { normal_ = normal; }
        void SetTextureCoordinates(const Vector& t1, const Vector& t2, const Vector& t3);
        const Vector& GetNormal() const { return normal_; }

        bool operator==(const Triangle3D& right) const;

    private:
        unsigned int p1_;
        unsigned int p2_;
        unsigned int p3_;

        Vector normal_;

        Vector p1TextureCordinates_;
        Vector p2TextureCordinates_;
        Vector p3TextureCordinates_;
};

struct Triangle2D
{
    Vector p1_;
    Vector p2_;
    Vector p3_;

    Triangle2D(const Vector& p1, const Vector& p2, const Vector& p3);

    bool operator==(const Triangle2D right) const;
};

#endif
