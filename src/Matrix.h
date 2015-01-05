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
        Matrix(const Matrix& other);
        Matrix(Matrix&& other);
        ~Matrix();

        double GetElement(const int& row, const int& col) const;
        void SetElement(const int& row, const int& col, const double& val);
        unsigned int GetWidth() const { return width_; }
        unsigned int GetHeight() const { return height_; }

        bool operator==(const Matrix& right);
        Matrix& operator=(const Matrix& right);
        Matrix operator*(double number) const;
        Matrix& operator*=(double number);
        Matrix operator*(const Matrix& right) const;
        Matrix operator-(const Matrix& right) const;

        Matrix Invert3x3Matrix() const;

        void Print() const;

        static void Multiply(const Matrix& m1, const Matrix& m2, Matrix& result);

        static void Set3x3Matrix(Matrix& m, double m11, double m12, double m13, double m21, double m22, double m23, double m31, double m32, double m33);

        static Matrix CreateIdentityMatrix(int size);
        static Matrix Create4x4Matrix(double m11, double m12, double m13, double m14,
            double m21, double m22, double m23, double m24,
            double m31, double m32, double m33, double m34,
            double m41, double m42, double m43, double m44);
        static Matrix Create3x3Matrix(double m11, double m12, double m13, double m21, double m22, double m23, double m31, double m32, double m33);
        static Matrix Create3x1Matrix(double m11, double m21, double m31);
        static Matrix CreateScaleMatrix(double xFactor, double yFactor,
            double zFactor);
        static Matrix CreateTranslationMatrix(double xMove, double yMove,
            double zMove);
        static Matrix CreateTranslationMatrix(const Vector& v);
        static Matrix CreateXAxisRotationMatrix(double angleInRadians);
        static Matrix CreateXAxisRotationMatrixAroundPoint(double angleInRadians,
                const Vector& p);
        static Matrix CreateYAxisRotationMatrix(double angleInRadians);
        static Matrix CreateYAxisRotationMatrixAroundPoint(double angleInRadians,
                const Vector& p);
        static Matrix CreateZAxisRotationMatrix(double angleInRadians);
        static Matrix CreateZAxisRotationMatrixAroundPoint(double angleInRadians,
                const Vector& p);
        static Matrix CreatePerspectiveProjectionMatrix();
        static Matrix CreateOrthogonalProjectionMatrix();

        static Matrix CreatePerspectiveProjectionMatrix2(double viewAngleRad,
            double aspect, double znear, double zfar);
        static Matrix CreateViewMatrix(const Vector& cameraPosition,
            const Vector& observedPoint, const Vector& upDirection);

    private:
        double* matrix_;
        const unsigned int width_;
        const unsigned int height_;

        double Det() const;
};

#endif
