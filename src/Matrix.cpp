#include "Matrix.h"
#include "Vector.h"

Matrix semiInvertedMatrix(3, 3);

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

Matrix& Matrix::operator*=(double number)
{
    for (unsigned int row = 0; row < height_; row++)
        for (unsigned int col = 0; col < width_; col++)
            SetElement(row, col, GetElement(row, col) * number);

    return *this;
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
    semiInvertedMatrix.SetElement(0, 0, GetElement(1, 1) * GetElement(2, 2) - GetElement(1, 2) * GetElement(2, 1));
    semiInvertedMatrix.SetElement(0, 1, GetElement(0, 2) * GetElement(2, 1) - GetElement(0, 1) * GetElement(2, 2));
    semiInvertedMatrix.SetElement(0, 2, GetElement(0, 1) * GetElement(1, 2) - GetElement(0, 2) * GetElement(1, 1));
    semiInvertedMatrix.SetElement(1, 0, GetElement(1, 2) * GetElement(2, 0) - GetElement(1, 0) * GetElement(2, 2));
    semiInvertedMatrix.SetElement(1, 1, GetElement(0, 0) * GetElement(2, 2) - GetElement(0, 2) * GetElement(2, 0));
    semiInvertedMatrix.SetElement(1, 2, GetElement(0, 2) * GetElement(1, 0) - GetElement(0, 0) * GetElement(1, 2));
    semiInvertedMatrix.SetElement(2, 0, GetElement(1, 0) * GetElement(2, 1) - GetElement(1, 1) * GetElement(2, 0));
    semiInvertedMatrix.SetElement(2, 1, GetElement(0, 1) * GetElement(2, 0) - GetElement(0, 0) * GetElement(2, 1));
    semiInvertedMatrix.SetElement(2, 2, GetElement(0, 0) * GetElement(1, 1) - GetElement(0, 1) * GetElement(1, 0));

    return semiInvertedMatrix * (1 / det);
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


Matrix Matrix::Create4x4Matrix(double m11, double m12, double m13, double m14,
        double m21, double m22, double m23, double m24,
        double m31, double m32, double m33, double m34,
        double m41, double m42, double m43, double m44)
{
    Matrix m(4, 4);
    m.matrix_[0] =  m11;
    m.matrix_[1] =  m12;
    m.matrix_[2] =  m13;
    m.matrix_[3] =  m14;

    m.matrix_[4] =  m21;
    m.matrix_[5] =  m22;
    m.matrix_[6] =  m23;
    m.matrix_[7] =  m24;

    m.matrix_[8] =  m31;
    m.matrix_[9] =  m32;
    m.matrix_[10] =  m33;
    m.matrix_[11] =  m34;

    m.matrix_[12] =  m41;
    m.matrix_[13] =  m42;
    m.matrix_[14] =  m43;
    m.matrix_[15] =  m44;

    return m;
}
Matrix Matrix::Create3x3Matrix(double m11, double m12, double m13, double m21, double m22, double m23, double m31, double m32, double m33)
{
    Matrix m(3, 3);

    m.matrix_[0] = m11;
    m.matrix_[1] = m12;
    m.matrix_[2] =  m13;

    m.matrix_[3] =  m21;
    m.matrix_[4] =  m22;
    m.matrix_[5] =  m23;

    m.matrix_[6] =  m31;
    m.matrix_[7] =  m32;
    m.matrix_[8] =  m33;

    return m;
}

void Matrix::Set3x3Matrix(Matrix& m, double m11, double m12, double m13, double m21, double m22, double m23, double m31, double m32, double m33)
{
    m.matrix_[0] = m11;
    m.matrix_[1] = m12;
    m.matrix_[2] =  m13;

    m.matrix_[3] =  m21;
    m.matrix_[4] =  m22;
    m.matrix_[5] =  m23;

    m.matrix_[6] =  m31;
    m.matrix_[7] =  m32;
    m.matrix_[8] =  m33;
}

Matrix Matrix::Create3x1Matrix(double m11, double m21, double m31)
{
    Matrix m(1, 3);

    m.matrix_[0] =  m11;
    m.matrix_[1] =  m21;
    m.matrix_[2] =  m31;

    return m;
}

Matrix Matrix::CreateScaleMatrix(double xFactor, double yFactor, double zFactor)
{
    return Create4x4Matrix(xFactor, 0, 0, 0,
                              0, yFactor, 0, 0,
                              0, 0, zFactor, 0,
                              0, 0, 0, 1);
}

Matrix Matrix::CreateTranslationMatrix(double xMove, double yMove, double zMove)
{
    return Create4x4Matrix(1, 0, 0, xMove,
                          0, 1, 0, yMove,
                          0, 0, 1, zMove,
                          0, 0, 0, 1);
}

Matrix Matrix::CreateTranslationMatrix(const Vector& v)
{
    return CreateTranslationMatrix(v.GetX(), v.GetY(), v.GetZ());
}

Matrix Matrix::CreateXAxisRotationMatrix(double radians)
{
    return Create4x4Matrix(1, 0, 0, 0,
          0, cos(radians), -sin(radians), 0,
          0, sin(radians), cos(radians), 0,
          0, 0, 0, 1);
}

Matrix Matrix::CreateXAxisRotationMatrixAroundPoint(double angleInRadians,
    const Vector& p)
{
    return CreateTranslationMatrix(p) * CreateXAxisRotationMatrix(angleInRadians) * CreateTranslationMatrix(-p);
}

Matrix Matrix::CreateYAxisRotationMatrix(double radians)
{
    return Create4x4Matrix(cos(radians), 0, sin(radians), 0,
                                                  0, 1, 0, 0,
                                                  -sin(radians), 0, cos(radians), 0,
                                                  0, 0, 0, 1);
}

Matrix Matrix::CreateYAxisRotationMatrixAroundPoint(double angleInRadians,
    const Vector& p)
{
    return CreateTranslationMatrix(p) * CreateYAxisRotationMatrix(angleInRadians) * CreateTranslationMatrix(-p);
}

Matrix Matrix::CreateZAxisRotationMatrix(double radians)
{
    return Create4x4Matrix(cos(radians), -sin(radians), 0, 0,
            sin(radians), cos(radians), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);

}

Matrix Matrix::CreateZAxisRotationMatrixAroundPoint(double angleInRadians,
    const Vector& p)
{
    return CreateTranslationMatrix(p) * CreateZAxisRotationMatrix(angleInRadians) * CreateTranslationMatrix(-p);
}

Matrix Matrix::CreatePerspectiveProjectionMatrix(int viewportWidth, int viewportHeight)
{
    return 
        CreateTranslationMatrix(viewportWidth / 2, viewportHeight / 2, 0) *
        CreateScaleMatrix(viewportWidth / 2, viewportHeight / 2, 1) * 
        Create4x4Matrix(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 1, 0);
}

Matrix Matrix::CreateOrthogonalProjectionMatrix(int viewportWidth, int viewportHeight)
{
    return 
        CreateTranslationMatrix(viewportWidth / 2, viewportHeight / 2, 0) *
        CreateScaleMatrix(viewportWidth / 2, viewportHeight / 2, 1) * 
        Create4x4Matrix(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 1);
}

Matrix Matrix::CreatePerspectiveProjectionMatrix2(double viewAngleRad,
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

Matrix Matrix::CreateViewMatrix(const Vector& cameraPosition,
        const Vector& observedPoint, const Vector& upDirection)
{
    Vector cameraVector = Vector(0, 0, 0) - cameraPosition;
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
