#include "Matrix.h"

bool DoubleEquals(double d1, double d2)
{
    return std::abs(d1 - d2) < 0.0001;
}

Matrix::Matrix(const int& n): Matrix(n, n)
{
}

Matrix::Matrix(const int& width, const int& height): matrix_(height, width)
{
}

Matrix::Matrix(const std::vector<std::vector<double>>& matrix):
    Matrix(matrix[0].size(), matrix.size())
{
    for (int i = 0; i < GetHeight(); i++)
        for (int j = 0; j < GetWidth(); j++)
            matrix_(i, j) = matrix[i][j];
}

Matrix::~Matrix()
{
}

Matrix::Matrix(Matrix&& other)
{
    matrix_ = other.matrix_;
}

Matrix::Matrix(const Matrix& other)
{
}

double Matrix::GetElement(const int& row, const int& col) const
{
    return matrix_(row, col);
}

void Matrix::SetElement(const int& row, const int& col, const double& val)
{
    matrix_(row, col) = val;
}

bool Matrix::operator==(const Matrix& right)
{
    if (GetWidth() == right.GetWidth() && GetHeight() == right.GetHeight())
    {
        for (unsigned int i = 0; i < GetHeight(); i++)
            for (unsigned int j = 0; j < GetWidth(); j++)
                if (GetElement(i, j) != right.GetElement(i, j))
                    return false;

        return true;
    }
    else
        return false;
}


void Matrix::Multiply(const Matrix& left, const Matrix& right, Matrix& result)
{
    result.matrix_ = left.matrix_ * right.matrix_;
}

Matrix Matrix::operator*(double number) const
{
    Matrix result(width_, height_);

    for (unsigned int row = 0; row < height_; row++)
        for (unsigned int col = 0; col < width_; col++)
            result.SetElement(row, col, GetElement(row, col) * number);

    return result;
}

Matrix Matrix::operator-(const Matrix& right) const
{
    Matrix result(width_, height_);

    for (unsigned int row = 0; row < height_; row++)
        for (unsigned int col = 0; col < width_; col++)
            result.SetElement(row, col, GetElement(row, col) - right.GetElement(row, col));

    return result;
}

Matrix Matrix::Invert3x3Matrix() const
{
    double det = Det();
    std::vector<std::vector<double>> matrixData  {
        {
            GetElement(1, 1) * GetElement(2, 2) - GetElement(1, 2) * GetElement(2, 1),
            GetElement(0, 2) * GetElement(2, 1) - GetElement(0, 1) * GetElement(2, 2),
            GetElement(0, 1) * GetElement(1, 2) - GetElement(0, 2) * GetElement(1, 1)
        },
        {
            GetElement(1, 2) * GetElement(2, 0) - GetElement(1, 0) * GetElement(2, 2),
            GetElement(0, 0) * GetElement(2, 2) - GetElement(0, 2) * GetElement(2, 0),
            GetElement(0, 2) * GetElement(1, 0) - GetElement(0, 0) * GetElement(1, 2)
        },
        {
            GetElement(1, 0) * GetElement(2, 1) - GetElement(1, 1) * GetElement(2, 0),
            GetElement(0, 1) * GetElement(2, 0) - GetElement(0, 0) * GetElement(2, 1),
            GetElement(0, 0) * GetElement(1, 1) - GetElement(0, 1) * GetElement(1, 0)
        }
    };
    Matrix semiInverted(matrixData);
    return semiInverted * (1 / det);
}

void Matrix::Print() const
{
    for (unsigned int i = 0; i < height_; i++)
    {
        for (unsigned int j = 0; j < width_; j++)
        {
            std::cout.width(7);
            std::cout << GetElement(i, j);
        }
        std::cout << std::endl;
    }
}

double Matrix::Det() const
{
    double d1 = GetElement(0, 0) * GetElement(1, 1) * GetElement(2, 2);
    double d2 = GetElement(0, 1) * GetElement(1, 2) * GetElement(2, 0);
    double d3 = GetElement(0, 2) * GetElement(1, 0) * GetElement(2, 1);
    double d4 = GetElement(0, 0) * GetElement(1, 2) * GetElement(2, 1);
    double d5 = GetElement(0, 2) * GetElement(1, 1) * GetElement(2, 0);
    double d6 = GetElement(0, 1) * GetElement(1, 0) * GetElement(2, 2);

    double det = d1 + d2 + d3 - d4 - d5 - d6;

    return det;
}

Matrix Matrix::CreateScaleMatrix(double xFactor, double yFactor, double zFactor)
{
    std::vector<std::vector<double>> matrixData { { xFactor, 0, 0, 0 },
                                                  { 0, yFactor, 0, 0 },
                                                  { 0, 0, zFactor, 0 },
                                                  { 0, 0, 0, 1} };

    return Matrix(matrixData);
}

Matrix Matrix::CreateTranslationMatrix(double xMove, double yMove, double zMove)
{
    std::vector<std::vector<double>> matrixData { { 1, 0, 0, xMove },
                                                  { 0, 1, 0, yMove },
                                                  { 0, 0, 1, zMove },
                                                  { 0, 0, 0, 1} };

    return Matrix(matrixData);
}

Matrix Matrix::CreateXAxisRotationMatrix(double angleInDegrees)
{
    double radians = angleInDegrees * M_PI / 180;
    std::vector<std::vector<double>> matrixData { { 1, 0, 0, 0 },
                                                  { 0, cos(radians), -sin(radians), 0 },
                                                  { 0, sin(radians), cos(radians), 0 },
                                                  { 0, 0, 0, 1} };

    return Matrix(matrixData);
}

Matrix Matrix::CreateYAxisRotationMatrix(double angleInDegrees)
{
    double radians = angleInDegrees * M_PI / 180;
    std::vector<std::vector<double>> matrixData { { cos(radians), 0, sin(radians), 0 },
                                                  { 0, 1, 0, 0 },
                                                  { -sin(radians), 0, cos(radians), 0 },
                                                  { 0, 0, 0, 1} };

    return Matrix(matrixData);
}

Matrix Matrix::CreateZAxisRotationMatrix(double angleInDegrees)
{
    double radians = angleInDegrees * M_PI / 180;
    std::vector<std::vector<double>> matrixData { { cos(radians), -sin(radians), 0, 0 },
                                                  { sin(radians), cos(radians), 0, 0 },
                                                  { 0, 0, 1, 0 },
                                                  { 0, 0, 0, 1} };

    return Matrix(matrixData);
}

Matrix Matrix::CreateProjectMatrix(double zDistance)
{
    std::vector<std::vector<double>> matrixData { { 1, 0, 0, 0 },
                                                  { 0, 1, 0, 0 },
                                                  { 0, 0, 0, 0 },
                                                  { 0, 0, 1 / zDistance, 1} };

    return Matrix(matrixData);
}

