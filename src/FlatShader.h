#ifndef FLAT_SHADING_H
#define FLAT_SHADING_H

#include <QColor>

#include "Point.h"
#include "Vector.h"
#include "Material.h"

struct TriangleShadingInfo
{
    Point p1;
    Point p2;
    Point p3;
    Vector p1Normal;
    Vector p2Normal;
    Vector p3Normal;

    Vector triangleNormal;

    Point observatorPosition;
    Point lightPosition;
    QColor lightColor;
    Material material;
};

class FlatShader
{
    public:
        FlatShader(const TriangleShadingInfo& shadingInfo);
        QColor GetColorForPixel(const Point& pixel) const;

    protected:
        TriangleShadingInfo shadingInfo_;
        Vector lightVector_;
};

#endif
