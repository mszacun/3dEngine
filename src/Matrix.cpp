#include "Matrix.h"

bool DoubleEquals(double d1, double d2)
{
    return std::abs(d1 - d2) < 0.0001;
}

Matrix::Matrix(const int& n): Matrix(n, n)
{
}

Matrix::Matrix(const int& width, const int& height): width_(width), height_(height)
{
    matrix_ = new double[width * height];
}

Matrix::Matrix(const std::vector<std::vector<double>>& matrix):
    Matrix(matrix[0].size(), matrix.size())
{
    for (unsigned int i = 0; i < height_; i++)
        for (unsigned int j = 0; j < width_; j++)
            matrix_[i * width_ + j] = matrix[i][j];
}

Matrix::~Matrix()
{
    delete[] matrix_;
}

Matrix::Matrix(const Matrix& other): width_(0), height_(0)
{
}

double Matrix::GetElement(const int& row, const int& col) const
{
    return matrix_[row * width_ + col];
}

bool Matrix::operator==(const Matrix& right)
{
    if (width_ == right.width_ && height_ == right.height_)
    {
        for (unsigned int i = 0; i < height_; i++)
            for (unsigned int j = 0; j < width_; j++)
                if (GetElement(i, j) != right.GetElement(i, j))
                    return false;

        return true;
    }
    else
        return false;
}


void Matrix::Multiply(const Matrix& left, const Matrix& right, Matrix& result)
{
    unsigned int height = left.height_;
    unsigned int width = right.width_;
    memset( result.matrix_, 0x0, width * height * sizeof(double) );

     for( unsigned int i = 0; i < left.height_; i++ ) {
         for( unsigned int k = 0; k < right.height_; k++ )
         {
             const double* a = left.matrix_ + i * left.width_ + k;
             const double* b = right.matrix_ + k * right.width_;
             double* c = result.matrix_ + i * width;

             double* cMax = c + width;
             while( c < cMax )
             {
                 *c++ += (*a) * (*b++);
             }
         }
     }
 }

