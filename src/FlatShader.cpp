#include "FlatShader.h"
#include <iostream>


QColor operator*(const QColor& left, double right)
{
    return QColor(ClipRGB(left.red() * right),
            ClipRGB(left.green() * right),
            ClipRGB(left.blue() * right));
}

QColor operator+(const QColor& left, const QColor& right)
{
    return QColor(std::min(left.red() + right.red(), 255),
            std::min(left.green() + right.green(), 255),
            std::min(left.blue() + right.blue(), 255));
}

void Shader::InitShader(const TriangleShadingInfo& shadingInfo)
{
    shadingInfo_ = shadingInfo;
    shadingInfo_.lightColor.getRgb(lightRgb, lightRgb +1, lightRgb + 2, lightRgb + 3);
    shadingInfo_.ambientLightColor.getRgb(ambientLightRgb, ambientLightRgb +1,
            ambientLightRgb + 2, ambientLightRgb + 3);

    textureCoordinatesInterpolator_.SetVector1(Vector((int) shadingInfo_.projectedP1.GetX(),
                (int) shadingInfo_.projectedP1.GetY(), 1), shadingInfo_.p1TextureCoordinates);
    textureCoordinatesInterpolator_.SetVector2(Vector((int) shadingInfo_.projectedP2.GetX(),
                (int) shadingInfo_.projectedP2.GetY(), 1), shadingInfo_.p2TextureCoordinates);
    textureCoordinatesInterpolator_.SetVector3(Vector((int) shadingInfo_.projectedP3.GetX(),
                (int) shadingInfo_.projectedP3.GetY(), 1), shadingInfo_.p3TextureCoordinates);
}

QColor Shader::CalculatePhongModel(const Vector& point, const Vector& lightVector,
       const Vector& orgNormal) const
{
    int result[4];
    Vector textureCoordinates = shadingInfo_.material->IsConstant() && shadingInfo_.normalModifier->IsConstant() ?
        Vector(0, 0, 0) : textureCoordinatesInterpolator_.Interpolate(point);
    Vector normal = shadingInfo_.normalModifier->ModifyNormal(orgNormal, textureCoordinates);

    // difuse
    double cos = normal.Dot(lightVector);
    if (cos <= 0.0)
        return QColor(0, 0, 0);

    // specular
    Vector toObserverVector = (shadingInfo_.observatorPosition - point).Normalize();
    Vector reflectionVector = (normal * 2 * normal.Dot(toObserverVector) - toObserverVector);
    double dot = std::max(0.0, lightVector.Dot(reflectionVector));
    double exponent = std::pow(dot, shadingInfo_.material->GetShiness());

    for (int i = 0; i < 3; i++)
    {
        double diffuse = lightRgb[i] * cos * shadingInfo_.material->GetDiffuse(i, textureCoordinates);
        double specular = shadingInfo_.material->GetSpecular(i, textureCoordinates) * lightRgb[i] * exponent;
        double own = shadingInfo_.material->GetOwnLigth(i, textureCoordinates);
        result[i] = diffuse + specular + own + ambientLightRgb[i];
    }

    return QColor(ClipRGB(result[0]), ClipRGB(result[1]), ClipRGB(result[2]));
}

void FlatShader::InitShader(const TriangleShadingInfo& shadingInfo)
{
    Shader::InitShader(shadingInfo);
    double xCenter = (shadingInfo_.p1.GetX() + shadingInfo_.p2.GetX() + shadingInfo_.p3.GetX()) / 3;
    double yCenter = (shadingInfo_.p1.GetY() + shadingInfo_.p2.GetY() + shadingInfo_.p3.GetY()) / 3;
    double zCenter = (shadingInfo_.p1.GetZ() + shadingInfo_.p2.GetZ() + shadingInfo_.p3.GetZ()) / 3;
    Vector triangleCenter = Vector(xCenter, yCenter, zCenter);
    lightVector_  = shadingInfo_.lightPosition - triangleCenter;
    lightVector_ = lightVector_.Normalize();

    calculatedColor_ =  CalculatePhongModel(triangleCenter, lightVector_, shadingInfo_.triangleNormal);
}

QColor FlatShader::GetColorForPixel(const Vector& pixel) const
{
    return calculatedColor_;
}

void GouraudShader::InitShader(const TriangleShadingInfo& shadingInfo)
{
    Shader::InitShader(shadingInfo);
    Vector lightVector1 = (shadingInfo_.lightPosition - shadingInfo_.p1).Normalize();
    Vector lightVector2 = (shadingInfo_.lightPosition - shadingInfo_.p2).Normalize();
    Vector lightVector3 = (shadingInfo_.lightPosition - shadingInfo_.p3).Normalize();

    p1Color_ = CalculatePhongModel(shadingInfo_.p1,
            lightVector1, shadingInfo_.p1Normal);            
    p2Color_ = CalculatePhongModel(shadingInfo_.p2,
            lightVector2, shadingInfo_.p2Normal);            
    p3Color_ = CalculatePhongModel(shadingInfo_.p3,
            lightVector3, shadingInfo_.p3Normal);            

    interpolator.SetVector1(Vector((int) shadingInfo_.projectedP1.GetX(), (int) shadingInfo_.projectedP1.GetY(), 1), p1Color_);
    interpolator.SetVector2(Vector((int) shadingInfo_.projectedP2.GetX(), (int) shadingInfo_.projectedP2.GetY(), 1), p2Color_);
    interpolator.SetVector3(Vector((int) shadingInfo_.projectedP3.GetX(), (int) shadingInfo_.projectedP3.GetY(), 1), p3Color_);
}

QColor GouraudShader::GetColorForPixel(const Vector& pixel) const
{
    return interpolator.Interpolate(Vector(pixel.GetX(), pixel.GetY(), 1));
}

void PhongShader::InitShader(const TriangleShadingInfo& shadingInfo)
{
    Shader::InitShader(shadingInfo);
    normalInterpolator.SetVector1(Vector(shadingInfo_.projectedP1.GetX(), shadingInfo_.projectedP1.GetY(), 1),
            shadingInfo_.p1Normal);
    normalInterpolator.SetVector2(Vector(shadingInfo_.projectedP2.GetX(), shadingInfo_.projectedP2.GetY(), 1),
            shadingInfo_.p2Normal);
    normalInterpolator.SetVector3(Vector(shadingInfo_.projectedP3.GetX(), shadingInfo_.projectedP3.GetY(), 1),
            shadingInfo_.p3Normal);

    Vector lightVector1 = (shadingInfo_.lightPosition - shadingInfo_.p1).Normalize();
    Vector lightVector2 = (shadingInfo_.lightPosition - shadingInfo_.p2).Normalize();
    Vector lightVector3 = (shadingInfo_.lightPosition - shadingInfo_.p3).Normalize();
    lightVectorInterpolator.SetVector1(Vector(shadingInfo_.projectedP1.GetX(), shadingInfo_.projectedP1.GetY(), 1),
            lightVector1);
    lightVectorInterpolator.SetVector2(Vector(shadingInfo_.projectedP2.GetX(), shadingInfo_.projectedP2.GetY(), 1),
            lightVector2);
    lightVectorInterpolator.SetVector3(Vector(shadingInfo_.projectedP3.GetX(), shadingInfo_.projectedP3.GetY(), 1),
            lightVector3);
}

QColor PhongShader::GetColorForPixel(const Vector& pixel) const
{
    Vector pointNormal =  normalInterpolator.Interpolate(Vector(pixel.GetX(), pixel.GetY(), 1)).Normalize();
    Vector lightVector =  lightVectorInterpolator.Interpolate(Vector(pixel.GetX(), pixel.GetY(), 1)).Normalize();
    return CalculatePhongModel(pixel, lightVector, pointNormal);            
}
