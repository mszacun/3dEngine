#ifndef CAMERA_H
#define CAMERA_H

#include "Vector.h"
#include "Matrix.h"

class Camera
{
    public:
        Camera(const Vector& pos, const Vector& up, const Vector& tar, double zmin,
            double zmax) : position(pos), upDirection(up), target(tar),
            zmin(zmin), zmax(zmax) {}
                
        Vector position;
        Vector upDirection;
        Vector target;

        double zmin;
        double zmax;

        void Transform(const Matrix& transformationMatrix);

        virtual double GetMaxX() const = 0;
        virtual double GetMaxY() const = 0;
};

class PerspectiveCamera : public Camera
{
    public:
        PerspectiveCamera(const Vector& pos, const Vector& up, const Vector& tar, double zmin,
            double zmax, double viewAngle) : Camera(pos, up, tar, zmin, zmax), viewAngle_(viewAngle) 
        {
        }

        double GetMaxX() const override { return (zmax * std::tan(viewAngle_ / 2)); }
        double GetMaxY() const override { return (zmax * std::tan(viewAngle_ / 2)); }

    protected:
        double viewAngle_;

};

class OrthogonalCamera : public Camera
{
    public:
        OrthogonalCamera(const Vector& pos, const Vector& up, const Vector& tar, double zmin,
            double zmax, double width, double height) : Camera(pos, up, tar, zmin, zmax),
            width_(width), height_(height)
        {
        }

        double GetMaxX() const override { return width_; }
        double GetMaxY() const override { return height_; }

    private:
        double width_;
        double height_;

};

#endif
