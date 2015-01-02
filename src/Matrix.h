#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <cstring>
#include <cmath>
#include <iostream>

class Point;
class Vector;

class Matrix
{
    public:
        Matrix(const int& n);
        Matrix(const int& width, const int& height);
        Matrix(const std::vector<std::vector<double>>& matrix);
        Matrix(Matrix&& other);
        ~Matrix();

        double GetElement(const int& row, const int& col) const;
        void SetElement(const int& row, const int& col, const double& val);

        bool operator==(const Matrix& right);
        Matrix operator*(double number) const;
        Matrix operator-(const Matrix& right) const;

        Matrix Invert3x3Matrix() const;

        void Print() const;

        static void Multiply(const Matrix& m1, const Matrix& m2, Matrix& result);

        static Matrix CreateIdentityMatrix(int size);
        static Matrix CreateScaleMatrix(double xFactor, double yFactor,
            double zFactor);
        static Matrix CreateTranslationMatrix(double xMove, double yMove,
            double zMove);
        static Matrix CreateXAxisRotationMatrix(double angleInDegrees);
        static Matrix CreateYAxisRotationMatrix(double angleInDegrees);
        static Matrix CreateZAxisRotationMatrix(double angleInDegrees);
        static Matrix CreateProjectMatrix(double zDistance);

        static Matrix CreatePerspectiveProjectionMatrix(double viewAngleRad,
            double aspect, double znear, double zfar);
        static Matrix CreateViewMatrix(const Point& cameraPosition,
            const Vector& observedPoint, const Vector& upDirection);

    private:
        double* matrix_;
        const unsigned int width_;
        const unsigned int height_;

        Matrix(const Matrix& other);
        double Det() const;
};

#endif
