#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <cstring>
#include <cmath>

class Matrix
{
    public:
        Matrix(const int& n);
        Matrix(const int& width, const int& height);
        Matrix(const std::vector<std::vector<double>>& matrix);
        ~Matrix();

        double GetElement(const int& row, const int& col) const;

        bool operator==(const Matrix& right);

        static void Multiply(const Matrix& m1, const Matrix& m2, Matrix& result);

    private:
        double* matrix_;
        const unsigned int width_;
        const unsigned int height_;

        Matrix(const Matrix& other);
};

#endif
