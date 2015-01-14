#ifndef FLAT_SHADING_H
#define FLAT_SHADING_H

#include <functional>
#include <QColor>

#include "Vector.h"
#include "Material.h"
#include "Interpolator.h"
#include "NormalModifier.h"

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
    NormalModifier* normalModifier;

    Vector p1TextureCoordinates;
    Vector p2TextureCoordinates;
    Vector p3TextureCoordinates;

    Vector triangleNormal;

    Vector observatorPosition;
    Vector lightPosition;
    QColor lightColor;
    QColor ambientLightColor;
    MaterialPtr material;
};

class Shader
{
    public:
        virtual ~Shader() {}
        virtual void InitShader(const TriangleShadingInfo& shadingInfo);
        virtual QColor GetColorForPixel(const Vector& pixel) const = 0;

    protected:
        TriangleShadingInfo shadingInfo_;
        Vector lightVector_;
        int lightRgb[4];
        int ambientLightRgb[4];

        Triangle3DInterpolator<Vector> textureCoordinatesInterpolator_;

        QColor CalculatePhongModel(const Vector& point, const Vector& lightVector,
            const Vector& normal) const;
};

class FlatShader : public Shader
{
    public:
        virtual void InitShader(const TriangleShadingInfo& shadingInfo) override;
        QColor GetColorForPixel(const Vector& pixel) const override;

    protected:
        QColor calculatedColor_;
};

class GouraudShader : public Shader
{
    public:
        virtual void InitShader(const TriangleShadingInfo& shadingInfo) override;
        QColor GetColorForPixel(const Vector& pixel) const override;

    private:
        QColor p1Color_;
        QColor p2Color_;
        QColor p3Color_;

        Triangle3DInterpolator<QColor> interpolator;
};

class PhongShader : public Shader
{
    public:
        virtual void InitShader(const TriangleShadingInfo& shadingInfo) override;
        QColor GetColorForPixel(const Vector& pixel) const override;

    private:
        Triangle3DInterpolator<Vector> interpolator;
};

#endif
