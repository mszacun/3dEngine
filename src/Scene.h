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
        ~Scene3D();

        void AddPoint(const int& x, const int& y, const int& z);
        void AddPoint(const Point& p);
        void AddTriangle(const int& p1, const int& p2, const int& p3);
        void AddTriangle(Triangle3D& triangle);

        std::vector<Point> GetPoints() const { return points_; }
        std::vector<Triangle3D> GetTriangles() const { return triangles_; }

        Vector CalculateNormal(const Triangle3D& triangle) const;
        Vector CalculatePointNormal(unsigned int pointNumber) const;
        void RecalculateNormals();

        void SetObserverPosition(const Point& newPosition);
        void SetLightPosition(const Point& newPosition);
        void SetLightColor(const QColor& color);

        Scene2D GetPerspectiveProjection() const;
        QImage RenederPerspectiveProjection(int width, int height);

        void DrawTriangleWithXParellGround(const Point& p1, const Point& p2,
            const Point& p3, QPainter& painter, FlatShader& shader);
        void DrawTriangle(const Point& p1, const Point& p2, const Point& p3,
                QPainter& painter, FlatShader& shader);

        void Transform(const Matrix& transformationMatrix);

    private:
        std::vector<Point> points_;
        std::vector<Triangle3D> triangles_;
        std::vector<Vector> pointsNormals_;

        Point observatorPosition_;
        Point lightPosition_;
        Vector observedPoint_;
        Vector upDirection_;
        QColor lightColor_;

        double** zBuffer_;
        Matrix worldTransformation_;

        Triangle2D ProjectTrianglePerspectively(const Triangle3D& triangle,
                const Matrix& transformationMatrix) const;
        void PrintProjectInfo(const Triangle3D& t, const Triangle2D& t2) const;

        Matrix CreateViewMatrix() const;
        Matrix CreatePerspectiveProjectionMatrix(double viewAngleRad,
            double aspect, double znear, double zfar) const;

};

#endif
