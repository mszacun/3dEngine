#include "Scene.h"
#include <cassert>

#define ZBUFFER_WIDTH 1024
#define ZBUFFER_HEIGHT 1024
void ClearZBuffer(double** zBuffer);

void Scene2D::AddTriangle(const Triangle2D& triangle)
{
    triangles_.push_back(triangle);
}

Scene3D::Scene3D() : observatorPosition_(0, 0, 0), observedPoint_(0, 0, 0),
    upDirection_(0, 1, 0), worldTransformation_(4, 4)
{
    zBuffer_ = new double*[ZBUFFER_HEIGHT];

    for (int i = 0; i < ZBUFFER_HEIGHT; i++)
        zBuffer_[i] = new double[ZBUFFER_WIDTH];

    worldTransformation_ = Matrix::CreateIdentityMatrix(4);
}

Scene3D::~Scene3D()
{
//    for (int i = 0; i < ZBUFFER_HEIGHT; i++)
//        delete[] zBuffer_[i];

//    delete[] zBuffer_;
}

void Scene3D::AddPoint(const int& x, const int& y, const int& z)
{
    points_.push_back(Point(x, y, z));
}

void Scene3D::AddPoint(const Point& p)
{
    points_.push_back(p);
}

void Scene3D::AddTriangle(const int& p1, const int& p2, const int& p3)
{
    Triangle3D triangle(p1, p2, p3);
    AddTriangle(triangle);
}

void Scene3D::AddTriangle(Triangle3D& triangle)
{
    triangle.SetNormal(CalculateNormal(triangle));
    triangles_.push_back(triangle);
}

Vector Scene3D::CalculateNormal(const Triangle3D& triangle) const
{
    Vector v1(points_[triangle.GetP1()], points_[triangle.GetP2()]);
    Vector v2(points_[triangle.GetP2()], points_[triangle.GetP3()]);

    return v1.Cross(v2).Normalize();
}

Vector Scene3D::CalculatePointNormal(unsigned int pointNumber) const
{
    Vector sum(0, 0, 0);

    for (const Triangle3D& t : triangles_)
    {
        if (t.GetP1() == pointNumber || t.GetP2() == pointNumber ||
                t.GetP3() == pointNumber)
            sum = sum + t.GetNormal();
    }

    return sum.Normalize();
}

void Scene3D::RecalculateNormals()
{
    pointsNormals_.clear();
    for (unsigned int i = 0; i < points_.size(); i++)
        pointsNormals_.push_back(CalculatePointNormal(i));
}

void Scene3D::SetObserverPosition(const Point& newPosition)
{
    observatorPosition_ = newPosition;
}

void Scene3D::SetLightPosition(const Point& newPosition)
{
    lightPosition_ = newPosition;
}

void Scene3D::SetLightColor(const QColor& color)
{
    lightColor_ = color;
}

Scene2D Scene3D::GetPerspectiveProjection() const
{
    Scene2D result;
    Matrix transformationMatrix = worldTransformation_ * Matrix::CreateProjectMatrix(-observatorPosition_.GetZ());

    for (const Triangle3D& t : triangles_)
        result.AddTriangle(ProjectTrianglePerspectively(t, transformationMatrix));

    return result;
}

void SortVertices(const Point& p1, const Point& p2, const Point& p3,
        Point& left, Point& center, Point& right)
{
    if (p1.GetX() < p2.GetX())
    {
        if (p1.GetX() < p3.GetX())
        {
            left = p1;
            if (p3.GetX() < p2.GetX())
            {
                center = p3;
                right = p2;
            }
            else
            {
                center = p2;
                right = p3;
            }
        }
        else
        {
            left = p3;
            center = p1;
            right = p2;
        }
    }
    else
    {
        if (p2.GetX() < p3.GetX())
        {
            left = p2;
            if (p3.GetX() < p1.GetX())
            {
                center = p3;
                right = p1;
            }
            else
            {
                center = p1;
                right = p3;
            }
        }
        else
        {
            left = p3;
            center = p2;
            right = p1;
        }
    }
}

double CalculateDeltaY(double y, double ystart, double yend)
{
    return (ystart == yend) ? 1 : (y - yend) / (ystart - yend);
}

void Scene3D::DrawTriangleWithXParellGround(const Point& p1, const Point& p2, const Point& p3, QPainter& painter, FlatShader& shader)
{
    // inv: p2 and p3 are on the same line, p1 is peak of triangle
    assert(p2.GetY() == p3.GetY());

    // in case of problems try changing to double
    int ystart = (int) p1.GetY();
    int yend = (int) p2.GetY();
    int direction = ystart < yend ? 1 : -1;
    for (int y = ystart; y != yend + direction; y += direction)
    {
        double betay = CalculateDeltaY(y, ystart, yend);
        double xl = betay * p1.GetX() + (1 - betay) * p2.GetX();
        double xr = betay * p1.GetX() + (1 - betay) * p3.GetX();

        for (int x = (int) xl; x < xr; x++)
        {
            painter.setPen(shader.GetColorForPixel(Point(x, y, 0)));
            painter.drawPoint(x, y);
        }

    }
}

void Scene3D::DrawTriangle(const Point& p1, const Point& p2, const Point& p3, QPainter& painter, FlatShader& shader)
{
    // inv: p2 is on the left of p3
    Point left, right, center;
    SortVertices(p1, p2, p3, left, center, right);
    if (left.GetY() == right.GetY())
        DrawTriangleWithXParellGround(center, left, right, painter, shader);
    else
    {
        if (left.GetY() < right.GetY())
        {
            double y = left.GetY();
            double betay = CalculateDeltaY(y, center.GetY(), right.GetY());
            double xr = betay * center.GetX() + (1 - betay) * right.GetX();

            DrawTriangleWithXParellGround(center, left, Point(xr, left.GetY(), 0), painter, shader);
            DrawTriangleWithXParellGround(right, left, Point(xr, left.GetY(), 0), painter, shader);
        }
        else
        {
            double y = right.GetY();
            double betay = CalculateDeltaY(y, center.GetY(), left.GetY());
            double xl = betay * center.GetX() + (1 - betay) * left.GetX();

            DrawTriangleWithXParellGround(center, Point(xl, right.GetY(), 0), right, painter, shader);
            DrawTriangleWithXParellGround(left, Point(xl, right.GetY(), 0), right, painter, shader);
        }
    }

}

void Scene3D::PrintProjectInfo(const Triangle3D& t, const Triangle2D& t2) const
{
    std::cout << "Projectiing trinagle" << std::endl;
    std::cout << points_[t.GetP1()] << " -> " << t2.p1_ << std::endl;
}

QImage Scene3D::RenederPerspectiveProjection(int width, int height)
{
    QImage result(width, height, QImage::Format_ARGB32);
    QPainter painter(&result);

    painter.fillRect(0, 0, 200, 200, QColor("white"));
    ClearZBuffer(zBuffer_);

    Matrix transformationMatrix = Matrix::CreateProjectMatrix(-observatorPosition_.GetZ()) * worldTransformation_;

    std::cout << "Transformation matrix: " << std::endl;
    transformationMatrix.Print();
/*    for (const Triangle3D& t : triangles_)
    {
        Triangle2D t2 = ProjectTrianglePerspectively(t, transformationMatrix);
        PrintProjectInfo(t, t2);
        TriangleShadingInfo shadingInfo;

        shadingInfo.p1 = points_[t.GetP1()];
        shadingInfo.p2 = points_[t.GetP2()];
        shadingInfo.p3 = points_[t.GetP3()];

        shadingInfo.p1Normal = pointsNormals_[t.GetP1()];
        shadingInfo.p2Normal = pointsNormals_[t.GetP2()];
        shadingInfo.p3Normal = pointsNormals_[t.GetP3()];

        shadingInfo.triangleNormal = t.GetNormal();

        shadingInfo.observatorPosition = observatorPosition_;
        shadingInfo.lightPosition = lightPosition_;
        shadingInfo.lightColor = lightColor_;

        FlatShader shader(shadingInfo);
        //DrawTriangle(t2.p1_, t2.p2_, t2.p3_, painter, shader);
        painter.drawLine((int) t2.p1_.GetX(), (int) t2.p1_.GetY(), (int) t2.p2_.GetX(), (int) t2.p2_.GetY());
        painter.drawLine((int) t2.p1_.GetX(), (int) t2.p1_.GetY(), (int) t2.p3_.GetX(), (int) t2.p3_.GetY());
        painter.drawEllipse((int) t2.p1_.GetX(), (int) t2.p1_.GetY(), 1, 1);
    }*/
    for (const Point& p : points_)
    {
         Point transformed = p.Transform(transformationMatrix);
         std::cout << "Original: " << p << " transformed: " << transformed << std::endl;
         double x = transformed.GetX()/* * width + width / 2*/;
         double y = transformed.GetY()/* * height + height / 2*/;
         painter.setBrush(QColor("gold"));
         painter.drawEllipse((int) x, (int) y, 1, 1);
    }

    return result;
}

void Scene3D::AccumulateTransformation(const Matrix& transformationMatrix)
{
    worldTransformation_ = transformationMatrix * worldTransformation_;
}

void Scene3D::Transform(const Matrix& transformationMatrix)
{
    for (Point& p : points_)
        p = p.Transform(transformationMatrix);
}

Triangle2D Scene3D::ProjectTrianglePerspectively(const Triangle3D& triangle,
        const Matrix& transformationMatrix) const
{
     return Triangle2D(points_[triangle.GetP1()].Transform(transformationMatrix),
         points_[triangle.GetP2()].Transform(transformationMatrix),
         points_[triangle.GetP3()].Transform(transformationMatrix));
}

void ClearZBuffer(double** zBuffer)
{
    for (int i = 0; i < ZBUFFER_HEIGHT; i++)
        for (int j = 0; j < ZBUFFER_WIDTH; j++)
            zBuffer[i][j] = -666;
}
