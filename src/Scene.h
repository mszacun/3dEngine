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
#include "NormalModifier.h"
#include "Camera.h"

struct OrthogonalProjection
{
    QImage renderedImage;
    PerspectiveCameraFrustrum perspectiveCameraFrustrum;
};

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
        void AddTexture(int triangleNumber, const Vector& t1, const Vector& t2,
                const Vector& t3);

        const std::vector<Vector>& GetPoints() const { return points_; }
        const std::vector<Triangle3D>& GetTriangles() const { return triangles_; }
        MaterialPtr GetMaterial() const { return material_; }

        Vector CalculateNormal(const Triangle3D& triangle) const;
        Vector CalculatePointNormal(unsigned int pointNumber) const;
        Vector CalculateCenter(const Triangle3D& triangle) const;
        void RecalculateNormals();

        void SetLightPosition(const Vector& newPosition);
        void SetLightColor(const QColor& color);
        void SetAmbientLightColor(const QColor& color) { ambientLightColor_ = color; }
        void SetMaterial(MaterialPtr material) { material_ = material; }

        QImage RenderProjection(int width, int height,
                const PerspectiveCamera& camera, Shader& shader,
                NormalModifier& normalModifier);
        OrthogonalProjection RenderProjection(int width, int height, const OrthogonalCamera& camera,
            Shader& shader, PerspectiveCameraFrustrum frustrum,
            NormalModifier& normalModifier);

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
        MaterialPtr material_;

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
                const Camera& camera, const ZInterpolator& zinterpolator,
                Shader& shader, NormalModifier& normalModifier);
        void DrawProjectedTriangle(QPainter& painter, const Triangle3D& t,
            const Matrix& transformationMatrix, const Camera& camera,
            const ZInterpolator& zinterpolator, Shader& shader,
            NormalModifier& normalModifier);

        void SortTriangles();

        void ViewTransform(Camera& camera);
};

typedef std::shared_ptr<Scene3D> ScenePtr;

#endif
