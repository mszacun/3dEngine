#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <unordered_set>

#include "Point.h"
#include "Triangle.h"

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
        void AddTriangle(const int& p1, const int& p2, const int& p3);

        void SetObserverPosition(const Point& newPosition);

        Scene2D GetPerspectiveProjection() const;

        void Transform(const Matrix& transformationMatrix);

    private:
        std::vector<Point> points_;
        std::vector<Triangle3D> triangles_;

        Point observatorPosition_;

        Triangle2D ProjectTrianglePerspectively(const Triangle3D& triangle,
                const Matrix& transformationMatrix) const;

};

#endif
