#ifndef FLAT_SHADING_H
#define FLAT_SHADING_H

#include <QColor>

#include "Vector.h"
#include "Material.h"

struct TriangleShadingInfo
{
    Vector p1;
    Vector p2;
    Vector p3;
    Vector p1Normal;
    Vector p2Normal;
    Vector p3Normal;

    Vector triangleNormal;

    Vector observatorPosition;
    Vector lightPosition;
    QColor lightColor;
    Material material;
};

class FlatShader
{
    public:
        FlatShader(const TriangleShadingInfo& shadingInfo);
        QColor GetColorForPixel(const Vector& pixel) const;

    protected:
        TriangleShadingInfo shadingInfo_;
        Vector lightVector_;
};

#endif
