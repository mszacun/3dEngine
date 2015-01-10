#ifndef FLAT_SHADING_H
#define FLAT_SHADING_H

#include <QColor>

#include "Vector.h"
#include "Material.h"
#include "Interpolator.h"

struct TriangleShadingInfo
{
    Vector p1;
    Vector p2;
    Vector p3;

    Vector projectedP1;
    Vector projectedP2;
    Vector projectedP3;

    Vector p1Normal;
    Vector p2Normal;
    Vector p3Normal;

    Vector triangleNormal;

    Vector observatorPosition;
    Vector lightPosition;
    QColor lightColor;
    QColor ambientLightColor;
    Material material;
};

class Shader
{
    public:
        Shader(const TriangleShadingInfo& shadingInfo);
        virtual QColor GetColorForPixel(const Vector& pixel) const = 0;

    protected:
        TriangleShadingInfo shadingInfo_;
        Vector lightVector_;
        int lightRgb[4];
        int ambientLightRgb[4];

        QColor CalculatePhongModel(const Vector& point, const Vector& lightVector,
            const Vector& normal) const;
};

class FlatShader : public Shader
{
    public:
        FlatShader(const TriangleShadingInfo& shadingInfo);
        QColor GetColorForPixel(const Vector& pixel) const override;

    protected:
        QColor calculatedColor_;
};

class GouraudShader : public Shader
{
    public:
        GouraudShader(const TriangleShadingInfo& shadingInfo);
        QColor GetColorForPixel(const Vector& pixel) const override;

    protected:
        QColor p1Color_;
        QColor p2Color_;
        QColor p3Color_;

        Triangle3DInterpolator<QColor> interpolator;
};

class InterpolateShader : public GouraudShader
{
    public:
        InterpolateShader(const TriangleShadingInfo& shadingInfo, QColor c1, QColor c2, QColor c3);
};

class PhongShader : public Shader
{
    public:
        PhongShader(const TriangleShadingInfo& shadingInfo);
        QColor GetColorForPixel(const Vector& pixel) const override;

    private:
        Triangle3DInterpolator<Vector> interpolator;
};

#endif
