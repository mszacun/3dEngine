#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <unordered_set>
#include <QWidget>
#include <QPainter>

#include "Point.h"
#include "Vector.h"
#include "Triangle.h"
#include "FlatShader.h"

class Scene2D
{
    public:
        void AddTriangle(const Triangle2D& triangle);
        std::vector<Triangle2D> triangles_;
};

class Scene3D
{
    public:
        Scene3D();

        void AddPoint(const int& x, const int& y, const int& z);
        void AddPoint(const Point& p);
        void AddTriangle(const int& p1, const int& p2, const int& p3);
        void AddTriangle(const Triangle3D& triangle);

        std::vector<Point> GetPoints() const { return points_; }
        std::vector<Triangle3D> GetTriangles() const { return triangles_; }

        Vector CalculateNormal(const Triangle3D& triangle) const;
        Vector CalculatePointNormal(unsigned int pointNumber) const;
        void RecalculateNormals();

        void SetObserverPosition(const Point& newPosition);
        void SetLightPosition(const Point& newPosition);
        void SetLightColor(const QColor& color);

        Scene2D GetPerspectiveProjection() const;
        QImage RenederPerspectiveProjection() const;

        void Transform(const Matrix& transformationMatrix);

    private:
        std::vector<Point> points_;
        std::vector<Triangle3D> triangles_;
        std::vector<Vector> trianglesNormals_;
        std::vector<Vector> pointsNormals_;

        Point observatorPosition_;
        Point lightPosition_;
        QColor lightColor_;

        Triangle2D ProjectTrianglePerspectively(const Triangle3D& triangle,
                const Matrix& transformationMatrix) const;
        void PrintProjectInfo(const Triangle3D& t, const Triangle2D& t2) const;

};

#endif
