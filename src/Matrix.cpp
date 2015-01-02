#include "Matrix.h"
#include "Vector.h"
#include "Point.h"

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
    for (unsigned int i = 0; i < width_ * height_; i++)
        matrix_[i] = 0;
}

Matrix::Matrix(const std::vector<std::vector<double>>& matrix):
    Matrix(matrix[0].size(), matrix.size())
{
    for (unsigned int i = 0; i < height_; i++)
        for (unsigned int j = 0; j < width_; j++)
            matrix_[i * width_ + j] = matrix[i][j];
}

Matrix::Matrix(const Matrix& other) : Matrix(other.width_, other.height_)
{
    for (unsigned int i = 0; i < height_; i++)
        for (unsigned int j = 0; j < width_; j++)
            SetElement(i, j, other.GetElement(i, j));
}

Matrix::~Matrix()
{
    delete[] matrix_;
}

Matrix::Matrix(Matrix&& other): width_(other.width_), height_(other.height_)
{
    matrix_ = other.matrix_;

    other.matrix_ = nullptr;
}

double Matrix::GetElement(const int& row, const int& col) const
{
    return matrix_[row * width_ + col];
}

void Matrix::SetElement(const int& row, const int& col, const double& val)
{
    matrix_[row * width_ + col] = val;
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

Matrix& Matrix::operator=(const Matrix& right)
{
    if (this != &right)
    {
        delete[] matrix_;

        matrix_ = new double[right.width_ * right.height_];
        for (unsigned int i = 0; i < right.GetHeight(); i++)
            for (unsigned int j = 0; j < right.GetWidth(); j++)
                SetElement(i, j, right.GetElement(i, j));
    }
    return *this;
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

Matrix Matrix::operator*(double number) const
{
    Matrix result(width_, height_);

    for (unsigned int row = 0; row < height_; row++)
        for (unsigned int col = 0; col < width_; col++)
            result.SetElement(row, col, GetElement(row, col) * number);

    return result;
}

Matrix Matrix::operator*(const Matrix& right) const
{
    Matrix result(GetHeight(), right.GetWidth());

    Multiply(*this, right, result);

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

Matrix Matrix::CreateIdentityMatrix(int size)
{
    Matrix result(size, size);

    for (int i = 0; i < size; i++)
    {
        result.SetElement(i, i, 1);
    }

    return result;
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

Matrix Matrix::CreatePerspectiveProjectionMatrix(double viewAngleRad,
        double aspect, double znear, double zfar)
{
    Matrix result(4, 4);
    double yScale = 1.0 / std::tan(viewAngleRad * 0.5);
    double xScale = yScale / aspect;

    double halfWidth = znear / xScale;
    double halfHeight = znear / yScale;
    double left = -halfWidth;
    double right = halfWidth;
    double bottom = -halfHeight;
    double top = halfHeight;

    double zRange = zfar / (zfar - znear);

    result.SetElement(0, 0, 2 * znear / (right - left));
    result.SetElement(1, 1, 2 * znear / (top - bottom));
    result.SetElement(2, 0, -(left + right) / (left - right));
    result.SetElement(2, 1, -(top + bottom) / (bottom - top));
    result.SetElement(2, 2, -zRange);
    result.SetElement(2, 3, -1);
    result.SetElement(3, 2, -znear * zRange);

    return result;
}

Matrix Matrix::CreateViewMatrix(const Point& cameraPosition,
        const Vector& observedPoint, const Vector& upDirection)
{
    Vector cameraVector(cameraPosition, Point(0, 0, 0));
    Vector zAxis = (observedPoint - cameraVector).Normalize();
    Vector xAxis = upDirection.Cross(zAxis).Normalize();
    Vector yAxis = zAxis.Cross(xAxis).Normalize();

    Matrix result = Matrix::CreateIdentityMatrix(4);

    result.SetElement(0, 0, xAxis.GetX());
    result.SetElement(0, 1, xAxis.GetY());
    result.SetElement(0, 2, xAxis.GetZ());

    result.SetElement(1, 0, yAxis.GetX());
    result.SetElement(1, 1, yAxis.GetY());
    result.SetElement(1, 2, yAxis.GetZ());

    result.SetElement(2, 0, zAxis.GetX());
    result.SetElement(2, 1, zAxis.GetY());
    result.SetElement(2, 2, zAxis.GetZ());

    result.SetElement(3, 0, -xAxis.Dot(cameraVector));
    result.SetElement(3, 1, -yAxis.Dot(cameraVector));
    result.SetElement(3, 2, -zAxis.Dot(cameraVector));

    return result;
}
