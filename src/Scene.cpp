#include "Scene.h"

void Scene2D::AddTriangle(const Triangle2D& triangle)
{
    triangles_.push_back(triangle);
}

Scene3D::Scene3D() : observatorPosition_(0, 0, 0)
{
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

void Scene3D::AddTriangle(const Triangle3D& triangle)
{
    triangles_.push_back(triangle);
    trianglesNormals_.push_back(CalculateNormal(triangle));
}

Vector Scene3D::CalculateNormal(const Triangle3D& triangle) const
{
    Vector v1(points_[triangle.GetP1()], points_[triangle.GetP2()]);
    Vector v2(points_[triangle.GetP2()], points_[triangle.GetP3()]);

    return v1.Cross(v2).Normalize();
}

Vector Scene3D::CalculatePointNormal(int pointNumber) const
{
    Vector sum(0, 0, 0);

    for (int i = 0; i < triangles_.size(); i++)
    {
        if (triangles_[i].GetP1() == pointNumber ||
                triangles_[i].GetP2() == pointNumber ||
                triangles_[i].GetP3() == pointNumber)
            sum = sum + trianglesNormals_[i];
    }

    return sum.Normalize();
}

void Scene3D::RecalculateNormals()
{
    pointsNormals_.clear();
    for (int i = 0; i < points_.size(); i++)
        pointsNormals_.push_back(CalculatePointNormal(i));
}

void Scene3D::SetObserverPosition(const Point& newPosition)
{
    observatorPosition_ = newPosition;
}

Scene2D Scene3D::GetPerspectiveProjection() const
{
    Scene2D result;
    Matrix transformationMatrix = Matrix::CreateProjectMatrix(-observatorPosition_.GetZ());

    for (const Triangle3D& t : triangles_)
        result.AddTriangle(ProjectTrianglePerspectively(t, transformationMatrix));

    return result;
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
