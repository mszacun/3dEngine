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
#include "Camera.h"


class Scene2D
{
    public:
        void AddTriangle(const Triangle2D& triangle);
        std::vector<Triangle2D> triangles_;
};

typedef std::function<double(double, double, double)> ZInterpolator;

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

        void SetLightPosition(const Vector& newPosition);
        void SetLightColor(const QColor& color);
        void SetAmbientLightColor(const QColor& color) { ambientLightColor_ = color; }

        QImage RenderProjection(int width, int height, const PerspectiveCamera& camera);
        QImage RenderProjection(int width, int height, const OrthogonalCamera& camert);

        void DrawTriangleWithXParellGround(const Vector& p1, Vector p2,
            Vector p3, QPainter& painter, Shader& shader, const ZInterpolator& zinterpolator);
        void DrawTriangle(const Vector& p1, const Vector& p2, const Vector& p3,
                QPainter& painter, Shader& shader, const ZInterpolator& zinterpolator);

        void AccumulateTransformation(const Matrix& transformationMatrix);
        void Transform(const Matrix& transformationMatrix, Camera& camera);

    private:
        std::vector<Vector> points_;
        std::vector<Triangle3D> triangles_;
        std::vector<Vector> pointsNormals_;

        Vector lightPosition_;

        QColor lightColor_;
        QColor ambientLightColor_;

        double** zBuffer_;
        Matrix worldTransformation_;

        Vector ProjectPoint(const Vector& p, const Matrix& projectionMatrix) const;
        Triangle2D ProjectTriangle(const Triangle3D& triangle,
                const Matrix& transformationMatrix) const;
        void PrintProjectInfo(const Triangle3D& t, const Triangle2D& t2) const;

        Matrix CreateViewMatrix() const;
        Matrix CreatePerspectiveProjectionMatrix(double viewAngleRad,
            double aspect, double znear, double zfar) const;

        void DrawScene(QPainter& painter, const Matrix& transformationMatrix,
                const Camera& camera, const ZInterpolator& zinterpolator);
        void DrawProjectedTriangle(QPainter& painter, const Triangle3D& t,
            const Matrix& transformationMatrix, const Camera& camera,
            const ZInterpolator& zinterpolator);

        void ViewTransform(Camera& camera);
};

#endif
