#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <limits>
#include <unordered_set>
#include <QWidget>
#include <QPainter>

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
        void AddPoint(const Vector& v);
        void AddTriangle(const int& p1, const int& p2, const int& p3);
        void AddTriangle(Triangle3D& triangle);

        std::vector<Vector> GetPoints() const { return points_; }
        std::vector<Triangle3D> GetTriangles() const { return triangles_; }

        Vector CalculateNormal(const Triangle3D& triangle) const;
        Vector CalculatePointNormal(unsigned int pointNumber) const;
        void RecalculateNormals();

        void SetObserverPosition(const Vector& newPosition);
        void SetObservedPoint(const Vector& newObservedPoint);
        void SetLightPosition(const Vector& newPosition);
        void SetLightColor(const QColor& color);

        Scene2D GetPerspectiveProjection() const;
        QImage RenederPerspectiveProjection(int width, int height);

        void DrawTriangleWithXParellGround(const Vector& p1, Vector p2,
            Vector p3, QPainter& painter, Shader& shader);
        void DrawTriangle(const Vector& p1, const Vector& p2, const Vector& p3,
                QPainter& painter, Shader& shader);

        void AccumulateTransformation(const Matrix& transformationMatrix);
        void Transform(const Matrix& transformationMatrix);

    private:
        std::vector<Vector> points_;
        std::vector<Triangle3D> triangles_;
        std::vector<Vector> pointsNormals_;

        Vector observatorPosition_;
        Vector lightPosition_;
        Vector observedPoint_;
        Vector upDirection_;
        QColor lightColor_;

        double** zBuffer_;
        Matrix worldTransformation_;

        Vector ProjectPoint(const Vector& p, const Matrix& projectionMatrix) const;
        Triangle2D ProjectTrianglePerspectively(const Triangle3D& triangle,
                const Matrix& transformationMatrix) const;
        void PrintProjectInfo(const Triangle3D& t, const Triangle2D& t2) const;

        Matrix CreateViewMatrix() const;
        Matrix CreatePerspectiveProjectionMatrix(double viewAngleRad,
            double aspect, double znear, double zfar) const;

        void DrawScene(QPainter& painter, const Matrix& transformationMatrix);
        void DrawProjectedTriangle(QPainter& painter, const Triangle3D& t, const Matrix& transformationMatrix);

        void ViewTransform();
};

#endif
