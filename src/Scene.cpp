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
    triangles_.push_back(triangle);
}

void Scene3D::AddTriangle(const Triangle3D& triangle)
{
    triangles_.push_back(triangle);
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
