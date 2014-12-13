#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <unordered_set>

#include "Point.h"
#include "Triangle.h"

class Scene3D
{
    public:
        Scene3D();

        void AddPoint(const int& x, const int& y, const int& z);
        void AddTriangle(const int& p1, const int& p2, const int& p3);

        void SetObserverPosition(const Point& newPosition);

    private:
        std::vector<Point> points_;
        std::vector<Triangle3D> triangles_;

        Point observatorPosition_;

};

#endif
